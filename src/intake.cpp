#include "main.h" // IWYU pragma: keep
#include "intake.hpp"
// Intake Motors/Sensors
pros::Motor bottomIntake(19, pros::MotorGears::blue);    // Port 19, 11W Blue Motor
pros::Motor middleIntake(-20, pros::MotorGears::green);  // Port 20 (reversed), 5.5W Green Motor
pros::Motor indexer(17, pros::MotorGears::green);        // Port 17, 5.5W Green Motor
pros::Optical intakeOptical(3);    // Port 3
pros::Distance intakeDistance(4);  // Port 4

// Pneumatics used in intake system
pros::adi::DigitalOut floatingPiston('A', false);  // Pneumatic piston on ADI port A
pros::adi::DigitalOut hoodPiston('B', false);      // Pneumatic piston on ADI port B

// Controller used during user control
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Toggles for intake control state tracking
bool floatingPistonToggle = false;  // Tracks floating piston state
bool hoodPistonToggle = false;      // Tracks hood piston state
int outtakeLongMode = 0;            // 0=forward only, 1=reverse red blocks, 2=reverse blue blocks

// Pneumatic control functions
void setFloatingPiston(bool extended) {
    floatingPistonToggle = extended;
    floatingPiston.set_value(extended);
}

void setHoodPiston(bool extended) {
    hoodPistonToggle = extended;
    hoodPiston.set_value(extended);
}

// Intake operation functions
void intakeStore(int voltage) {
    // Retract both pistons and run all intake motors forward
    setFloatingPiston(false);
    setHoodPiston(false);
    bottomIntake.move(voltage);
    middleIntake.move(voltage);
    indexer.move(voltage);
}

// Hue-based outtakeLong: checks intakeOptical to decide reverse logic
// Red objects (hue ~0-15 or ~345-360) reverse for 250ms, then forward
// Blue objects (hue ~200-250) go forward only
void outtakeLong(bool held, int voltage) {
    const int REVERSE_TICKS = 12;      // ~120ms at ~10ms loop
    const int MID_PHASE_TICKS = 37;   // ~370ms total (12 + 25 ticks)
    static int tick = 0;
    static int phase = 0; // 0=reverse, 1=mid phase, 2=forward

    if (!held) {
        tick = 0;
        phase = 0;
        return;
    }

    setFloatingPiston(false);
    setHoodPiston(true);

    // Read current hue
    int hue = intakeOptical.get_hue();
    bool isRed = (hue < 15 || hue > 345);
    bool isBlue = (hue > 200 && hue < 250);

    // Determine if we should use color sorting based on mode
    bool shouldSort = false;
    if (outtakeLongMode == 1 && isRed) {
        shouldSort = true;
    } else if (outtakeLongMode == 2 && isBlue) {
        shouldSort = true;
    }

    if (!shouldSort || outtakeLongMode == 0) {
        // Mode 0 or no matching color: just go forward
        bottomIntake.move(voltage);
        middleIntake.move(voltage);
        indexer.move(voltage);
    } else {
        // Three-phase sequence
        if (tick < REVERSE_TICKS) {
            // Phase 0: reverse all motors for 120ms
            phase = 0;
            bottomIntake.move(-voltage);
            middleIntake.move(-voltage);
            indexer.move(-voltage);
        } else if (tick < MID_PHASE_TICKS) {
            // Phase 1: bottom/middle forward, indexer reverse for 250ms
            phase = 1;
            bottomIntake.move(voltage);
            middleIntake.move(voltage);
            indexer.move(-voltage);
        } else {
            // Phase 2: all forward
            phase = 2;
            bottomIntake.move(voltage);
            middleIntake.move(voltage);
            indexer.move(voltage);
        }
    }

    tick++;
}

// One-shot overload: spawn background task for hue-based color sorting
void outtakeLong(int voltage) {
    static volatile bool taskRunning = false;
    static int taskVoltage = 0;

    if (taskRunning) return;
    taskVoltage = voltage;

    auto taskFn = [](void*) {
        taskRunning = true;
        setFloatingPiston(false);
        setHoodPiston(true);

        int hue = intakeOptical.get_hue();
        bool isRed = (hue < 15 || hue > 345);
        bool isBlue = (hue > 200 && hue < 250);

        // Determine if we should use color sorting based on mode
        bool shouldSort = false;
        if (outtakeLongMode == 1 && isRed) {
            shouldSort = true;
        } else if (outtakeLongMode == 2 && isBlue) {
            shouldSort = true;
        }

        if (shouldSort) {
            // Phase 1: reverse all motors for 120ms
            bottomIntake.move(-taskVoltage);
            middleIntake.move(-taskVoltage);
            indexer.move(-taskVoltage);
            pros::delay(120);

            // Phase 2: bottom/middle forward, indexer reverse for 250ms
            bottomIntake.move(taskVoltage);
            middleIntake.move(taskVoltage);
            indexer.move(-taskVoltage);
            pros::delay(250);
        }

        // Phase 3: all forward
        bottomIntake.move(taskVoltage);
        middleIntake.move(taskVoltage);
        indexer.move(taskVoltage);
        taskRunning = false;
    };

    new pros::Task(taskFn, nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "outtakeLong_one_shot");
}

// Performs a reverse pulse followed by forward motion while held
// Call with held=true every loop while L2 pressed; call with held=false to reset
void outtakeUpperMid(bool held, int voltage) {
    const int REVERSE_TICKS = 12; // ~120ms at typical loop frequency
    static int tick = 0;
    static bool didReverse = false;

    if (!held) {
        // Reset state when button released
        tick = 0;
        didReverse = false;
        return;
    }

    setFloatingPiston(false);
    setHoodPiston(false);

    if (!didReverse && tick < REVERSE_TICKS) {
        // Reverse phase: all motors reverse
        bottomIntake.move(-voltage);
        middleIntake.move(-voltage);
        indexer.move(-voltage);
    } else {
        // Forward phase: bottom/middle forward, indexer reverse
        didReverse = true;
        bottomIntake.move(voltage);
        middleIntake.move(voltage);
        indexer.move(-voltage);
    }

    tick++;
}

// Non-blocking one-shot outtake: reverse for 120ms then forward
void outtakeUpperMid(int voltage) {
    static volatile bool taskRunning = false;
    static int taskVoltage = 0;

    if (taskRunning) return; // Prevent overlapping calls
    
    taskVoltage = voltage;
    auto taskFn = [](void*) {
        taskRunning = true;
        setFloatingPiston(false);
        setHoodPiston(false);
        
        // Reverse phase
        bottomIntake.move(-taskVoltage);
        middleIntake.move(-taskVoltage);
        indexer.move(-taskVoltage);
        pros::delay(120);
        
        // Forward phase
        bottomIntake.move(taskVoltage);
        middleIntake.move(taskVoltage);
        indexer.move(-taskVoltage);
        
        taskRunning = false;
    };
    new pros::Task(taskFn, nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "outtakeUpperMid_one_shot");
}

void outtakeLowerMid(int voltage) {
    // Extend floating piston, retract hood piston, run all motors in reverse
    setFloatingPiston(true);
    setHoodPiston(false);
    bottomIntake.move(-voltage);
    middleIntake.move(-voltage);
    indexer.move(-voltage);
}

void outtakeDoublePark() {
    // Spin intake in reverse until block detected, then stop and extend piston
    setFloatingPiston(false);
    
    // Keep spinning reverse until distance < 8
    while (intakeDistance.get_distance() >= 8) {
        bottomIntake.move(-90);
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
            
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
                intakeStore(127);
            } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
                outtakeLowerMid(127);
            } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
                outtake(127);
            } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
                outtakeDoublePark();
            } else {
                intakeStop();
            }
        }
    }
}