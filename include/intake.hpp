#pragma once
#include "main.h" // IWYU pragma: keep
#include <sys/_intsup.h>

// Motor/Sensor declarations for intake system
extern pros::Motor bottomIntake;
extern pros::Motor middleIntake;
extern pros::Motor indexer;


// Pneumatic piston declarations
extern pros::adi::DigitalOut hoodPiston;

// Controller and state variables
extern pros::Controller master;
extern bool hoodPistonToggle;

// Piston control functions
void setHoodPiston(bool extended);

// Intake functions
void intakeStore(int voltage);
void intakeStop();

// Outtake functions
void outtakeLong(int voltage);
void outtakeUpperMid(int voltage); // convenience overload: acts as held=true
void outtakeMid(int voltage, int forwardDuration = 500, int reverseDuration = 200); // one-shot version for autons
void outtake(int voltage);

// Used control function
void intakeControl();