#include "main.h" // IWYU pragma: keep
#include "intake.hpp"

// Intake Motors/Sensors
pros::Motor bottomIntake(19, pros::MotorGears::blue);    // Port 19, 11W Blue Motor
pros::Motor middleIntake(17, pros::MotorGears::green);  // Port 17 (reversed), 5.5W Green Motor
pros::Motor indexer(-20, pros::MotorGears::green);        // Port 20, 5.5W Green Motor

// Pneumatics used in intake system
pros::adi::DigitalOut hoodPiston('A', false);      // Pneumatic piston on ADI port B

// Controller used during user control
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Toggles for intake control state tracking
bool hoodPistonToggle = false;      // Tracks hood piston state

// Pneumatic control functions
void setHoodPiston(bool extended) {
    hoodPistonToggle = extended;
    hoodPiston.set_value(extended);
}

// Variant that will stop the indexer motor once its actual velocity falls below threshold
// Intake storage function with optional indexer auto-stop
// When stopIndexerWhenSlow is true, monitors indexer velocity and stops it when a block is detected
void intakeStore(int voltage) {
    setHoodPiston(true); // Extend hood piston for intake position
    bottomIntake.move(voltage);
    middleIntake.move(voltage);
    indexer.move_velocity(150); // Use velocity control for smoother operation
}

// Hue-based outtakeLong: checks intakeOptical to decide reverse logic
// Red objects (hue ~0-15 or ~345-360) reverse for 200ms, then forward
// Blue objects (hue ~200-250) go forward only
// Continuously held outtakeLong: checks optical sensor hue to determine sorting behavior
void outtakeLong(int voltage) {
    setHoodPiston(false); // Retract hood piston for outtake position
    bottomIntake.move(voltage);
    middleIntake.move(voltage);
    indexer.move(voltage);
}


// Performs a reverse pulse followed by forward motion while held
// Call with held=true every loop while L2 pressed; call with held=false to reset
bool held = false;
void outtakeUpperMid( int voltage) {
    const int REVERSE_TICKS = 16; // ~160ms at typical loop frequency (~10ms per tick)
    static int tick = 0;          // Counts ticks to track phase timing
    static bool didReverse = false; // Tracks whether reverse phase completed

    if (!held) {
        // Reset state when button released
        tick = 0;
        didReverse = false;
        return;
    }

    held = true;  // Keep held state active while button is pressed
    
    // Retract both pistons for upper mid outtake position
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

// Blocking version for autonomous: does full reverse-then-forward sequence in one call
void outtakeMid(int voltage, int forwardDuration, int reverseDuration) {
    setHoodPiston(false);
    
    // Reverse phase: ~160ms
    bottomIntake.move(-voltage);
    middleIntake.move(-voltage);
    indexer.move(-voltage);
    pros::delay(reverseDuration);
    
    // Forward phase: bottom/middle forward, indexer reversed to hold back blocks
    bottomIntake.move(voltage);
    middleIntake.move(voltage);
    indexer.move(-voltage);
    pros::delay(forwardDuration);
    
    intakeStop();
}

void outtake(int voltage) {
    setHoodPiston(false);
    bottomIntake.move(-voltage);
    middleIntake.move(-voltage);
    indexer.move(-voltage);
}

void intakeStop() {
    held = false;
    setHoodPiston(false);
    bottomIntake.move(0);
    middleIntake.move(0);
    indexer.move(0);
}

void intakeControl() { 

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        intakeStore(127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        outtakeLong(127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        outtakeUpperMid(127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        outtake(127);
    } else {
        intakeStop();
    }

}
