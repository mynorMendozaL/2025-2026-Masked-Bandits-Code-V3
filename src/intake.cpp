#include "main.h" // IWYU pragma: keep
#include "intake.hpp"

// Intake Motors/Sensors
pros::Motor bottomIntake(19, pros::MotorGears::blue);    // Port 19, 11W Blue Motor
pros::Motor indexer(17, pros::MotorGears::blue);        // Port 17, 5.5W Green Motor

// Pneumatics used in intake system
pros::adi::DigitalOut floatingPiston('F', false);  // Pneumatic piston on ADI port A
pros::adi::DigitalOut indexerPiston('H', false);      // Pneumatic piston on ADI port B
pros::adi::DigitalOut hoodPiston('A', false);         // Pneumatic piston on ADI port C

// Controller used during user control
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Toggles for intake control state tracking
bool floatingPistonToggle = false;  // Tracks floating piston state
bool hoodPistonToggle = false;      // Tracks hood piston state
bool indexerPistonToggle = false;   // Tracks indexer piston state
// Pneumatic control functions
void setFloatingPiston(bool extended) {
    floatingPistonToggle = extended;
    floatingPiston.set_value(extended);
}

void setHoodPiston(bool extended) {
    hoodPistonToggle = extended;
    hoodPiston.set_value(extended);
}

void setIndexerPiston(bool extended) {
    indexerPistonToggle = extended;
    indexerPiston.set_value(extended);
}

void intakeStore(int voltage) {
    setFloatingPiston(true);
    setHoodPiston(false);
    setIndexerPiston(false);
    bottomIntake.move(voltage);
    indexer.move(voltage);
}

void outtakeLong(int voltage) {
    setFloatingPiston(true);
    setHoodPiston(true);
    setIndexerPiston(false);
    bottomIntake.move(voltage);
    indexer.move(voltage);
}

void outtakeUpperMid(int voltage) {
    setFloatingPiston(true);
    setHoodPiston(false);
    setIndexerPiston(true);
    bottomIntake.move(voltage);
    indexer.move(voltage);
}

void outtakeLowerMid(int voltage) {
    setFloatingPiston(false);
    setHoodPiston(false);
    setIndexerPiston(false);
    bottomIntake.move(-voltage);
    indexer.move(-voltage);
}

void outtake(int voltage) {
    // Retract both pistons and run all motors in reverse
    setFloatingPiston(true);
    setHoodPiston(false);
    setIndexerPiston(false);
    bottomIntake.move(-voltage);
    indexer.move(-voltage);
}

void intakeStop() {
    setFloatingPiston(true);
    setHoodPiston(false);
    bottomIntake.move(0);
    indexer.move(0);
}

void intakeControl() { 
    // Intake control logic can be implemented here
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        intakeStore(127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        outtakeLong(127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        outtakeUpperMid(127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        outtake(127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
        outtakeLowerMid(127);
    } else {
        intakeStop();
    }
}