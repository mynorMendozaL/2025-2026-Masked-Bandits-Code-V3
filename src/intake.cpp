#include "main.h" // IWYU pragma: keep
#include "intake.hpp"

// Intake Motors/Sensors
pros::Motor bottomIntake(19, pros::MotorGears::blue);    // Port 19, 11W Blue Motor
pros::Motor middleIntake(-20, pros::MotorGears::green);  // Port 20 (reversed), 5.5W Green Motor
pros::Motor indexer(17, pros::MotorGears::green);        // Port 17, 5.5W Green Motor
pros::Optical intakeOptical(7);    // Port 4
pros::Distance intakeDistance(3);  // Port 3

// Pneumatics used in intake system
pros::adi::DigitalOut floatingPiston('F', false);  // Pneumatic piston on ADI port A
pros::adi::DigitalOut hoodPiston('H', false);      // Pneumatic piston on ADI port B
pros::adi::DigitalOut doubleParkPiston('B', false); // Pneumatic piston on ADI port F

// Controller used during user control
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Toggles for intake control state tracking
bool floatingPistonToggle = false;  // Tracks floating piston state
bool hoodPistonToggle = false;      // Tracks hood piston state
int outtakeLongMode = 0;            // 0=forward only, 1=reverse red blocks, 2=reverse blue blocks
// Token to coordinate/interrupt one-shot intake tasks (increment to cancel)
volatile int intakeOneShotToken = 0;

// Pneumatic control functions
void setFloatingPiston(bool extended) {
    floatingPistonToggle = extended;
    floatingPiston.set_value(extended);
}

void setHoodPiston(bool extended) {
    hoodPistonToggle = extended;
    hoodPiston.set_value(extended);
}

// Variant that will stop the indexer motor once its actual velocity falls below threshold
// Intake storage function with optional indexer auto-stop
// When stopIndexerWhenSlow is true, monitors indexer velocity and stops it when a block is detected
void intakeStore(int voltage, bool stopIndexerWhenSlow) {
    const double INDEXER_VEL_THRESHOLD = 25; // Velocity threshold indicating block resistance/jam
    const int STARTUP_TICKS = 10; // ~100ms startup delay before velocity monitoring begins
    static bool indexerStopped = false; // True when indexer stopped due to low velocity detection
    static int tickCounter = 0; // Tracks elapsed ticks for startup delay

    // Retract pistons to intake storage position
    setFloatingPiston(false);
    setHoodPiston(false);

    // Run bottom and middle intake motors continuously
    bottomIntake.move(voltage);
    middleIntake.move(voltage);

    if (!stopIndexerWhenSlow) {
        // Standard mode: run indexer at constant velocity, reset tracking state
        indexerStopped = false;
        tickCounter = 0;
        indexer.move_velocity(200);
        return;
    }

    if (!indexerStopped) {
        // Auto-stop mode: monitor indexer velocity after startup period
        indexer.move_velocity(200);
        tickCounter++;
        
        // Check velocity after startup delay to avoid false triggers during acceleration
        if (tickCounter > STARTUP_TICKS) {
            double vel = indexer.get_actual_velocity();
            // Low velocity indicates block jam - stop indexer
            if (vel < INDEXER_VEL_THRESHOLD) {
                indexerStopped = true;
                indexer.move_velocity(50);
            }
        }
    } else {
        // Indexer stopped due to block detection - maintain stopped state
        indexer.move_velocity(50);
    }
}

// Hue-based outtakeLong: checks intakeOptical to decide reverse logic
// Red objects (hue ~0-15 or ~345-360) reverse for 200ms, then forward
// Blue objects (hue ~200-250) go forward only
// Continuously held outtakeLong: checks optical sensor hue to determine sorting behavior
void outtakeLong(bool held, int voltage) {
    const int REVERSE_TICKS = 20;      // ~200ms at ~10ms loop frequency
    static int tick = 0;               // Tracks elapsed ticks for phase timing

    if (!held) {
        // Reset state when button released
        tick = 0;
        return;
    }

    // Retract floating piston, extend hood piston for long outtake position
    setFloatingPiston(false);
    setHoodPiston(true);

    // Read current hue from optical sensor
    int hue = intakeOptical.get_hue();
    // Red objects have hue near 0 (wraps around 360)
    bool isRed = (hue < 15 || hue > 345);
    // Blue objects have hue around 200-260
    bool isBlue = (hue > 190 && hue < 260);

    // Determine if we should trigger color sorting based on current mode
    bool shouldSort = false;
    if (outtakeLongMode == 1 && isRed) {
        // Mode 1: Sort out (reject) red blocks
        shouldSort = true;
    } else if (outtakeLongMode == 2 && isBlue) {
        // Mode 2: Sort out (reject) blue blocks
        shouldSort = true;
    }

    if (!shouldSort || outtakeLongMode == 0) {
        // Mode 0 (no sorting) or no color match: run all motors forward
        bottomIntake.move(voltage);
        middleIntake.move(voltage);
        indexer.move(voltage);
    } else {
        // Color sorting active: two-phase sequence to reject unwanted block
        if (tick < REVERSE_TICKS) {
            // Phase 0 (Rejection): reverse indexer to push block back while intakes feed forward
            bottomIntake.move(voltage);
            middleIntake.move(voltage);
            indexer.move_velocity(-180);
        } else {
            // Phase 1 (Resume): all motors forward after rejection complete
            bottomIntake.move(voltage);
            middleIntake.move(voltage);
            indexer.move(voltage);
        }
    }

    tick++; // Increment tick counter for phase tracking
}

// Parameter struct and task functions for preemptible one-shot intake tasks
struct OneShotParams { int token; int voltage; };

static void outtakeLongTask(void* arg);
static void outtakeUpperMidTask(void* arg);
static void intakeStoreOnceTask(void* arg);

// One-shot overload: spawn background task for hue-based color sorting
void outtakeLong(int voltage) {
    // start one-shot that can be preempted by incrementing intakeOneShotToken
    OneShotParams* p = new OneShotParams{++intakeOneShotToken, voltage};
    new pros::Task(outtakeLongTask, p, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "outtakeLong_one_shot");
}

// Performs a reverse pulse followed by forward motion while held
// Call with held=true every loop while L2 pressed; call with held=false to reset
void outtakeUpperMid(bool held, int voltage) {
    const int REVERSE_TICKS = 16; // ~160ms at typical loop frequency (~10ms per tick)
    static int tick = 0;          // Counts ticks to track phase timing
    static bool didReverse = false; // Tracks whether reverse phase completed

    if (!held) {
        // Reset state when button released
        tick = 0;
        didReverse = false;
        return;
    }

    // Retract both pistons for upper mid outtake position
    setFloatingPiston(false);
    setHoodPiston(false);

    if (!didReverse && tick < REVERSE_TICKS) {
        // Reverse phase: run all motors backward to eject block upward
        bottomIntake.move(-voltage);
        middleIntake.move(-voltage);
        indexer.move(-voltage);
    } else {
        // Forward phase: bottom/middle forward to feed, indexer reverse to prevent double-feed
        didReverse = true;
        bottomIntake.move(voltage);
        middleIntake.move(voltage);
        indexer.move(-voltage);  // Keeps indexer reversed to hold back additional blocks
    } /* else {
        // Alternative forward phase with velocity control (commented out)
        didReverse = true;
        bottomIntake.move(voltage);
        middleIntake.move_velocity(150);
        indexer.move_velocity(-135);
    } */

    tick++; // Increment tick counter for phase tracking
}

// Non-blocking one-shot outtake: reverse for 120ms then forward
void outtakeUpperMid(int voltage) {
    // Use parameterized task to avoid lambda capture issues
    OneShotParams* p = new OneShotParams{++intakeOneShotToken, voltage};
    new pros::Task(outtakeUpperMidTask, p, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "outtakeUpperMid_one_shot");
}

// Lower mid outtake: extends floating piston and reverses motors to eject block
// Adjusts motor velocities based on input voltage for controlled ejection
void outtakeLowerMid(int voltage) {
    if (voltage <= 127) {
        // Normal voltage range
        // Extend floating piston, retract hood piston for lower mid position
        setFloatingPiston(true);
        setHoodPiston(false);
        bottomIntake.move(-voltage);  // Direct voltage control
        middleIntake.move(-voltage);           // Direct voltage control
        indexer.move(-voltage);                // Direct voltage control
    } else {
        // High voltage range: cap middle/indexer at max while allowing higher bottom speed
        setFloatingPiston(true);
        setHoodPiston(false);
        bottomIntake.move_velocity(-voltage);  // Use full requested velocity
        middleIntake.move(-127);               // Cap at maximum voltage
        indexer.move(-127);                    // Cap at maximum voltage
    }
}

void outtakeDoublePark() {
    // Spin intake in reverse until block detected, then stop and extend piston
    setFloatingPiston(false);
    
    // Keep spinning reverse until distance < 8
    while (intakeDistance.get_distance() >= 8) {
        bottomIntake.move_velocity(-200);
        middleIntake.move(-127);
        indexer.move(-127);
    }
    
    // Block detected, stop motors and extend piston
    bottomIntake.move(0);
    middleIntake.move(0);
    indexer.move(0);
    doubleParkPiston.set_value(true);
}

void outtake(int voltage) {
    // Retract both pistons and run all motors in reverse
    setFloatingPiston(false);
    setHoodPiston(false);
    bottomIntake.move(-voltage);
    middleIntake.move(-voltage);
    indexer.move(-voltage);
}

void intakeStop() {
    // Stop all intake motion and retract pistons
    ++intakeOneShotToken; // Cancel any running one-shot tasks
    setFloatingPiston(false);
    setHoodPiston(false);
    bottomIntake.move(0);
    middleIntake.move(0);
    indexer.move(0);
}

void intakeControl() { 
    // Cycle outtakeLongMode on LEFT button press: 0 -> 1 -> 2 -> 0
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        outtakeLongMode = (outtakeLongMode + 1) % 3;
        
        // Print current mode to controller
        if (outtakeLongMode == 0) {
            master.print(0, 0, "Mode: Forward Only");
        } else if (outtakeLongMode == 1) {
            master.print(0, 0, "Mode: Sort Red   ");
        } else if (outtakeLongMode == 2) {
            master.print(0, 0, "Mode: Sort Blue  ");
        }
        pros::delay(25);
    }
    
    // Handle L2: reverse-then-forward for upper mid
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        outtakeUpperMid(true, 127);
    } else {
        outtakeUpperMid(false, 0); // reset internal state
        
        // Handle L1: hue-based reverse-then-forward for long intake (allows drivetrain)
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            outtakeLong(true, 127);
        } else {
            outtakeLong(false, 0); // reset outtakeLong state
            
            // Reset intakeStore state on new R1 press
            if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
                intakeStore(0, false);
            }
            
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
                intakeStore(127, true);
            } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
                outtakeLowerMid(127);
            } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
                outtake(127);
            } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
                outtakeDoublePark();
            } else {
                intakeStop();
            }
        }
    }
}

// Non-blocking one-shot: run intake motors and stop indexer once its velocity drops
void intakeStoreOnce(int voltage) {
    OneShotParams* p = new OneShotParams{++intakeOneShotToken, voltage};
    new pros::Task(intakeStoreOnceTask, p, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "intakeStoreOnce");
}

// Task function implementations for preemptible one-shot behaviors
static void outtakeLongTask(void* arg) {
    OneShotParams* p = static_cast<OneShotParams*>(arg);
    if (!p) return;
    int localToken = p->token;
    int taskVoltage = p->voltage;
    delete p;

    const int REVERSE_TICKS = 20;    // ~150ms reverse

    setFloatingPiston(false);
    setHoodPiston(true);

    // Start motors forward and continuously check for wrong color
    bottomIntake.move(taskVoltage);
    middleIntake.move(taskVoltage);
    indexer.move(taskVoltage);

    //master.print(1, 0, "Sampling M:%d   ", outtakeLongMode);

    // Continuously monitor hue until preempted
    while (true) {
        if (intakeOneShotToken != localToken) {
            bottomIntake.move(0); middleIntake.move(0); indexer.move(0);
            //master.print(1, 0, "PREEMPTED       ");
            return;
        }
        
        int hue = intakeOptical.get_hue();
        bool isRed = (hue < 15 || hue > 345);
        bool isBlue = (hue > 190 && hue < 260);
        
        // Check if we detect wrong color and reject it
        if ((outtakeLongMode == 1 && isRed) || (outtakeLongMode == 2 && isBlue)) {
           // master.print(1, 0, "REJECT H:%d     ", hue);
            
            // Reverse indexer to push block back
            bottomIntake.move(taskVoltage);
            middleIntake.move(taskVoltage);
            indexer.move_velocity(-160);
            
            for (int t = 0; t < REVERSE_TICKS; ++t) {
                if (intakeOneShotToken != localToken) {
                    bottomIntake.move(0); middleIntake.move(0); indexer.move(0);
                    return;
                }
                pros::delay(10);
            }
            
            // Resume forward
            bottomIntake.move(taskVoltage);
            middleIntake.move(taskVoltage);
            indexer.move(taskVoltage);
        }
        
        pros::delay(10);
    }
}

static void outtakeUpperMidTask(void* arg) {
    OneShotParams* p = static_cast<OneShotParams*>(arg);
    if (!p) return;
    int localToken = p->token;
    int taskVoltage = p->voltage;
    delete p;

    setFloatingPiston(false);
    setHoodPiston(false);

    // Reverse phase
    bottomIntake.move(-taskVoltage);
    middleIntake.move(-taskVoltage);
    indexer.move(-taskVoltage);
    for (int t = 0; t < 16; ++t) {
        if (intakeOneShotToken != localToken) {
            bottomIntake.move(0); middleIntake.move(0); indexer.move(0);
            return;
        }
        pros::delay(10);
    }

    // Forward phase
    bottomIntake.move(taskVoltage);
    middleIntake.move(taskVoltage);
    indexer.move(-taskVoltage);
}

static void intakeStoreOnceTask(void* arg) {
    OneShotParams* p = static_cast<OneShotParams*>(arg);
    if (!p) return;
    int localToken = p->token;
    int taskVoltage = p->voltage;
    delete p;

    const double INDEXER_VEL_THRESHOLD = 25;
    const int STARTUP_DELAY_MS = 100; // Wait 100ms for motor to spin up

    setFloatingPiston(false);
    setHoodPiston(false);

    bottomIntake.move(taskVoltage);
    middleIntake.move(taskVoltage);
    indexer.move(taskVoltage);

    // Wait for motor to spin up before checking velocity
    pros::delay(STARTUP_DELAY_MS);

    while (true) {
        if (intakeOneShotToken != localToken) {
            bottomIntake.move(0); middleIntake.move(0); indexer.move(0);
            return;
        }
        double vel = indexer.get_actual_velocity();
        if (vel < INDEXER_VEL_THRESHOLD) break;
        pros::delay(10);
    }

    indexer.move(0);
    // leave bottom/middle running
}