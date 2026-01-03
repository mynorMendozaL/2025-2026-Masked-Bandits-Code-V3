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
    chassis.setPose(0, 0, 90);
    outtakeLongMode=2;
    chassis.moveToPoint(35.2, 0, 1000);
    pros::delay(50);
    setLittleWill(true);
    chassis.turnToHeading(176, 750, {.maxSpeed = 90});
    chassis.moveToPoint(35.6, -34, 1000, {.maxSpeed = 70}); //35.4
    intakeStoreOnce(127);
    pros::delay(975);
    chassis.moveToPoint(37.5, 20, 1000, {.forwards = false, .maxSpeed = 80}, false);
    outtakeLong(127);
    pros::delay(900);
    chassis.moveToPoint(37.5, -2, 1000);
    pros::delay(400);
    intakeStop();
    chassis.turnToHeading(311, 750, {.direction = lemlib::AngularDirection::CW_CLOCKWISE});
}