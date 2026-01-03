#include "lemlib/chassis/chassis.hpp" // IWYU pragma: keep
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h" // IWYU pragma: keep
#include "intake.hpp" // IWYU pragma: keep
#include "littleWill.hpp" // IWYU pragma: keep
#include "descore.hpp" // IWYU pragma: keep
#include "autons.hpp" // IWYU pragma: keep

extern lemlib::Chassis chassis; // declare chassis as extern
extern pros::MotorGroup left_motor_group;
extern pros::MotorGroup right_motor_group;

void right4Block() {
    //Setting current robot pose to (0,0) facing 90 degrees
    chassis.setPose(0, 0, 90);
    outtakeLongMode=2;

    //Move towards Match Loader
    chassis.moveToPoint(35.2, 0, 1000);
    pros::delay(50);
    setLittleWill(true);
    chassis.turnToHeading(176, 750, {.maxSpeed = 90});

    //Move to Match Loader and intake
    chassis.moveToPoint(35.6, -34, 1000, {.maxSpeed = 70});
    intakeStoreOnce(127);
    pros::delay(975);

    //Move backwards to Long Goal and score
    chassis.moveToPoint(37.5, 20, 1000, {.forwards = false, .maxSpeed = 80}, false);
    outtakeLong(127);
    pros::delay(900);
    chassis.moveToPoint(37.5, -2, 1000);
    pros::delay(400);
    intakeStop();
    setLittleWill(false);
    
    //Extend Wing then move to use Wing on Goal
    setWing(true);
    chassis.moveToPoint(47.8, 12, 1000, {.forwards = false});
    pros::delay(200);
    setWing(false);
    chassis.moveToPoint(47.8, 38, 1000, {.forwards = false});
}

void SAWP() {
    //Setting current robot pose to (0,0) facing 90 degrees
    chassis.setPose(0, 0, 90);
    outtakeLongMode=2;

    //Move towards Match Loader
    chassis.moveToPoint(35.2, 0, 1000);
    pros::delay(50);
    setLittleWill(true);
    chassis.turnToHeading(176, 750, {.maxSpeed = 90});

    //Move to Match Loader and intake
    chassis.moveToPoint(35.6, -34, 1000, {.maxSpeed = 70});
    intakeStoreOnce(127);
    pros::delay(975);

    //Move backwards to Long Goal and score
    chassis.moveToPoint(37.5, 20, 1000, {.forwards = false, .maxSpeed = 80}, false);
    outtakeLong(127);
    pros::delay(900);
    chassis.moveToPoint(37.5, -2, 1000);
    pros::delay(400);
    intakeStop();

    //Move towards 3 Block stack and intake
    chassis.turnToHeading(311, 750, {.direction = lemlib::AngularDirection::CW_CLOCKWISE});
    setLittleWill(false);
    chassis.moveToPoint(13.5, 18, 1000);
    intakeStoreOnce(127);
    pros::delay(500);
    setLittleWill(true);
    pros::delay(100);

    //Move towards Lower Middle Goal and score
    chassis.moveToPoint(1.5, 30, 1000);
    pros::delay(150);
    setLittleWill(false);
    pros::delay(300);
    outtakeLowerMid(100);
    pros::delay(500);

    //Move towards 2nd 3 Block stake and intake
    chassis.moveToPoint(15, 17, 1000, {.forwards = false});
    intakeStop();
    chassis.moveToPoint(-37, 17, 10000);
    pros::delay(1200);
    setLittleWill(true);
    pros::delay(100);

    //Move towards Upper Middle Goal and score
    chassis.moveToPoint(-23, 31, 1000, {.forwards = false}, false);
    outtakeUpperMid(127);
    pros::delay(500);
    chassis.moveToPoint(-37, 17, 1000);
}