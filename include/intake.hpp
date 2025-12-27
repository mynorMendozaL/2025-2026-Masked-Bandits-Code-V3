#pragma once
#include "main.h" // IWYU pragma: keep
#include <sys/_intsup.h>

// Motor/Sensor declarations for intake system
extern pros::Motor bottomIntake;
extern pros::Motor middleIntake;
extern pros::Motor indexer;
extern pros::Optical intakeOptical;
extern pros::Distance intakeDistance;

// Pneumatic piston declarations
extern pros::adi::DigitalOut floatingPiston;
extern pros::adi::DigitalOut hoodPiston;
extern pros::adi::DigitalOut doubleParkPiston;

// Controller and state variables
extern pros::Controller master;
extern bool floatingPistonToggle;
extern bool hoodPistonToggle;
extern int outtakeLongMode; // 0=forward only, 1=reverse red, 2=reverse blue

// Piston control functions
void setFloatingPiston(bool extended);
void setHoodPiston(bool extended);

// Intake functions
void intakeStore(int voltage);
void intakeStore(int voltage, bool stopIndexerWhenSlow); // if true, stop indexer when its actual velocity < 100
void intakeStoreOnce(int voltage); // non-blocking one-shot: runs intake and stops indexer when slow
void intakeStop();

// Outtake functions
void outtakeLong(bool held, int voltage);
void outtakeLong(int voltage); // convenience overload: one-shot
void outtakeUpperMid(bool held, int voltage);
void outtakeUpperMid(int voltage); // convenience overload: acts as held=true
void outtakeLowerMid(int voltage);
void outtakeDoublePark();
void outtake(int voltage);

// Used control function
void intakeControl();