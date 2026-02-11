#pragma once
#include "main.h" // IWYU pragma: keep
#include <sys/_intsup.h>

// Motor/Sensor declarations for intake system
extern pros::Motor bottomIntake;
extern pros::Motor middleIntake;
extern pros::Motor indexer;


// Pneumatic piston declarations
extern pros::adi::DigitalOut hoodPiston;
extern pros::adi::DigitalOut floatingPiston;

// Controller and state variables
extern pros::Controller master;
extern bool hoodPistonToggle;
extern bool floatingPistonToggle;

// Piston control functions
void setHoodPiston(bool extended);
void setFloatingPiston(bool extended);

// Intake functions
void intakeStore(int voltage);
void intakeStop();

// Outtake functions
void outtakeLong(int voltage);
void outtakeUpperMid(int voltage); // convenience overload: acts as held=true
void outtakeLowerMid(int voltage); 
void outtakeMid(int voltage, int forwardDuration = 500, int reverseDuration = 250); // one-shot version for autons
void outtake(int voltage);

// Used control function
void intakeControl();