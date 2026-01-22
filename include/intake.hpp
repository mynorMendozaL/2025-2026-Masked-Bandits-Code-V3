#pragma once
#include "main.h" // IWYU pragma: keep
#include <sys/_intsup.h>

// Motor/Sensor declarations for intake system
extern pros::Motor bottomIntake;
extern pros::Motor indexer;

// Pneumatic piston declarations
extern pros::adi::DigitalOut floatingPiston;
extern pros::adi::DigitalOut indexerPiston;
extern pros::adi::DigitalOut hoodPiston;

// Controller and state variables
extern pros::Controller master;
extern bool floatingPistonToggle;
extern bool indexerPistonToggle;
extern bool hoodPistonToggle;

// Piston control functions
void setFloatingPiston(bool extended);
void setHoodPiston(bool extended);

// Intake functions
void intakeStore(int voltage);
void intakeStop();

// Outtake functions
void outtakeLong(int voltage);
void outtakeUpperMid(int voltage);
void outtakeLowerMid(int voltage);
void outtake(int voltage);

// Used control function
void intakeControl();