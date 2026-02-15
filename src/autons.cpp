#include "lemlib/chassis/chassis.hpp" // IWYU pragma: keep
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h" // IWYU pragma: keep
#include "intake.hpp" // IWYU pragma: keep
#include "littleWill.hpp" // IWYU pragma: keep
#include "descore.hpp" // IWYU pragma: keep
#include "distanceReset.hpp" // IWYU pragma: keep
#include "autons.hpp" // IWYU pragma: keep

extern lemlib::Chassis chassis; // declare chassis as extern
extern pros::MotorGroup left_motor_group;
extern pros::MotorGroup right_motor_group;


void SAWP() {
    //Setting current robot pose to (0,0) facing 90 degrees
    chassis.setPose(0, 0, 90);

    //Move towards Match Loader
    chassis.moveToPoint(34.6, .25, 1000);
    pros::delay(50);
    chassis.turnToHeading(176, 500, {.maxSpeed = 90});//750
    pros::delay(75);
    setLittleWill(true);

    //Move to Match Loader and intake
    chassis.moveToPoint(34.6, -39.6, 1000, {.maxSpeed = 65});
    intakeStore(127);
    pros::delay(1100);

    //Move backwards to Long Goal and score
    chassis.moveToPoint(37.75, 20, 1000, {.forwards = false, .maxSpeed = 65}, false);
    outtake(127);
    pros::delay(100);
    outtakeLong(127);
    pros::delay(900);
    chassis.setPose(0,0,chassis.getPose().theta);
    pros::delay(5);
    chassis.swingToPoint(-29.25, 1.4, DriveSide::RIGHT, 750, {.minSpeed = 20, .earlyExitRange = 4});
    pros::delay(100);
    setLittleWill(false);
    /*chassis.moveToPoint(-0.55, -20, 1000);
    pros::delay(400);
    intakeStop();

    //Move towards 3 Block stack and intake
    chassis.turnToPoint(-27, 2.15, 750, {.direction = lemlib::AngularDirection::CW_CLOCKWISE, .minSpeed = 20, .earlyExitRange = 6});
    setLittleWill(false); */
    chassis.moveToPoint(-29.25, 1.4, 1000);
    intakeStore(100);
    pros::delay(475);//625
    setLittleWill(true);
    chassis.turnToPoint(-74, -4, 1000, {.minSpeed = 25, .earlyExitRange = 6});//-4
    chassis.moveToPoint(-74, -4, 10000);
    setLittleWill(false);
    pros::delay(865);
    setLittleWill(true);
    pros::delay(100);

    //Move towards Upper Middle Goal and score
    chassis.moveToPoint(-49, 24, 1000, {.forwards = false, .maxSpeed = 80}, false);//-50.5
    outtakeMid(127, 850, 350);
    chassis.moveToPose(-93.25, -51, 180, 2300, {.lead = 0.45, .maxSpeed = 75});
    intakeStore(127);
    chassis.moveToPoint(-95.5, 0, 1000, {.forwards = false, .maxSpeed = 80}, false);
    outtake(127);
    pros::delay(100);
    outtakeLong(127);
}

void right7Block() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(12.9, 26.75, 57, 1200, {.lead = 0.5}); //was moveToPoint
    intakeStore(127);
    pros::delay(850);
    setLittleWill(true);
    pros::delay(100);
    chassis.moveToPose(38, -26, 180, 1880, {.lead = 0.55});
    chassis.moveToPoint(37.1, 23, 1000, {.forwards = false, .maxSpeed = 70}, false);
    outtake(127);
    pros::delay(150);
    outtakeLong(127);
    pros::delay(1500);
    chassis.moveToPoint(48.125, 0, 1000);
    pros::delay(300);
    intakeStop();
    setLittleWill(false);
    chassis.turnToHeading(180, 500);
    chassis.moveToPoint(48.125, 35, 1000, {.forwards = false, .maxSpeed = 80});
}

void left7Block() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(-12.796, 26.75, -59, 1100, {.lead = 0.5}); //was moveToPoint
    intakeStore(127);
    pros::delay(775);
    setLittleWill(true);
    pros::delay(100);
    chassis.moveToPose(-36, -32, 180, 1950, {.lead = 0.5, . maxSpeed = 80});
    chassis.moveToPoint(-33.125, 27, 1000, {.forwards = false, .maxSpeed = 60}, false);
    outtake(127);
    pros::delay(150);
    outtakeLong(127);
    pros::delay(1550);
    chassis.moveToPoint(-20.25, 0, 1000);
    pros::delay(350);
    intakeStop();
    setLittleWill(false);
    chassis.turnToHeading(180, 500);
    chassis.moveToPoint(-20.75, 31, 1000, {.forwards = false, .maxSpeed = 80});
}

void skillsAuton() {
    //Setting current robot pose to (0,0) facing 90 degrees
    chassis.setPose(0, 0, 90);
    setWing(true);

    //Move towards Match Loader
    chassis.moveToPoint(35.5, .25, 1000);
    pros::delay(50);
    chassis.turnToHeading(176, 500, {.maxSpeed = 90});//750
    pros::delay(100);
    setLittleWill(true);

    //Move to Match Loader and intake
    chassis.moveToPoint(35.5, -39.9, 1000, {.maxSpeed = 65});
    intakeStore(127);
    pros::delay(2250);

    //Move towards other side of Long Goal
    chassis.moveToPoint(49, 4, 5000, {.forwards = false});
    intakeStop();
    setLittleWill(false);
    chassis.moveToPoint(49, 80, 5000, {.forwards = false});
    chassis.turnToHeading(45, 500);
    chassis.moveToPose(39, 65, 0, 1200, {.forwards = false, .lead = .5}, false);
    outtakeLong(127);
    setLittleWill(true);
    pros::delay(2000);
    chassis.moveToPoint(39, 110, 3000, {.maxSpeed = 60});
    pros::delay(200);
    intakeStore(127);
    pros::delay(2600);
    chassis.moveToPoint(39, 65, 1000, {.forwards = false, .maxSpeed = 80}, false);
    outtakeLong(127);
    pros::delay(2000);
    chassis.moveToPoint(39, 90, 1000);
    pros::delay(200);
    intakeStop();
    setLittleWill(false);
    chassis.turnToPoint(13.5, 63, 1000);
    chassis.moveToPoint(13.5, 63, 1000);
    intakeStore(127);
    pros::delay(700);
    setLittleWill(true);
    pros::delay(50);
    chassis.turnToPoint(-35, 62, 1000);
    chassis.moveToPoint(-35, 62, 10000);
    setLittleWill(false);
    pros::delay(700);
    setLittleWill(true);
}

void right43Block() {
    //Setting current robot pose to (0,0) facing 90 degrees
    chassis.setPose(0, 0, 90);

    //Move towards Match Loader
    chassis.moveToPoint(34.6, 0, 1000);
    chassis.turnToHeading(176, 750, {.maxSpeed = 90});
    pros::delay(100);
    setLittleWill(true);

    //Move to Match Loader and intake
    chassis.moveToPoint(35.75, -38, 1000, {.maxSpeed = 60});
    intakeStore(127);
    pros::delay(1000);

    //Move backwards to Long Goal and score
    chassis.moveToPoint(38.25, 20, 1000, {.forwards = false, .maxSpeed = 60}, false);
    outtakeLong(127);
    pros::delay(1250);
    chassis.moveToPoint(37.75, -5, 1000);
    pros::delay(400);
    intakeStop();

    //Move towards 3 Block stack and intake
    chassis.turnToPoint(10, 12.5, 750,  {.direction = lemlib::AngularDirection::CW_CLOCKWISE});
    setLittleWill(false);
    chassis.moveToPoint(10, 12.5, 1000, {.maxSpeed = 60});
    bottomIntake.move(127);
    pros::delay(825);
    setLittleWill(true);
    pros::delay(600);
    setLittleWill(false);

    //Move towards Lower Middle Goal and score
    chassis.moveToPoint(-2.25, 24, 1500, {.maxSpeed =  60});// X -2, y 23
    pros::delay(350);
    outtakeLowerMid(100);
    pros::delay(1700);

    //Move towards 2nd 3 Block stake and intake
    chassis.moveToPoint(26, 0, 1000, {.forwards = false});
    intakeStop();
    chassis.turnToHeading(0, 1000);
    chassis.moveToPoint(26, 26, 1000, {.maxSpeed = 90});
    chassis.turnToHeading(-55, 750);
}

void left43Block() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(-12.796, 26.75, -57, 1100, {.lead = 0.5}); //was moveToPoint
    intakeStore(127);
    pros::delay(800);
    setLittleWill(true);
    pros::delay(100);
    chassis.turnToHeading(-135, 1000);
    chassis.moveToPoint(7.25, 35.5, 1000, {.forwards = false}, false);
    outtakeMid(100, 1375, 350);
    chassis.moveToPose(-34, -58.5, 180, 2375, {.lead = 0.72, .maxSpeed = 70});
    intakeStore(127);
    chassis.moveToPoint(-31.6, 22.3, 1000, {.forwards = false, .maxSpeed = 70}, false);
    outtakeLong(127);
    pros::delay(750);
    chassis.moveToPoint(-20.5, 0, 1000);
    pros::delay(350);
    intakeStop();
    setLittleWill(false);
    chassis.turnToHeading(183, 500);
    chassis.moveToPoint(-19.75, 30, 1000, {.forwards = false});
}

void altleft43Block() {
    //Setting current robot pose to (0,0) facing 90 degrees
    chassis.setPose(0, 0, -90);

    //Move towards Match Loader
    chassis.moveToPoint(-34.6, 0, 1000);
    chassis.turnToHeading(-176, 600, {.maxSpeed = 90});
    pros::delay(100);
    setLittleWill(true);

    //Move to Match Loader and intake
    chassis.moveToPoint(-35.75, -39, 1000, {.maxSpeed = 60});
    intakeStore(127);
    pros::delay(950);

    //Move backwards to Long Goal and score
    chassis.moveToPoint(-34, 20, 1000, {.forwards = false, .maxSpeed = 60}, false);
    outtakeLong(127);
    setLittleWill(false);
    pros::delay(1250);
    chassis.moveToPoint(-30, -5, 1000);
    pros::delay(400);
    intakeStop();
    chassis.turnToPoint(0, 26, 500);
    chassis.moveToPoint(0, 26, 1000);
    intakeStore(127);
    pros::delay(650);
    setLittleWill(true);
    pros::delay(50);
    chassis.turnToHeading(-135, 500);
    chassis.moveToPoint(15, 40, 1000, {.forwards = false});
    outtakeMid(127, 1350, 350);
}

void altLeft7Block() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(-12.796, 26.75, -59, 1100, {.lead = 0.5}); //was moveToPoint
    intakeStore(127);
    pros::delay(775);
    setLittleWill(true);
    pros::delay(100);
    chassis.moveToPose(-36, -32, 180, 1950, {.lead = 0.5, . maxSpeed = 80});
    chassis.moveToPoint(-33.125, 27, 1000, {.forwards = false, .maxSpeed = 60}, false);
    outtake(127);
    pros::delay(150);
    outtakeLong(127);
    pros::delay(1550);
    chassis.moveToPoint(-33.125, 0, 1000);
    pros::delay(350);
    intakeStop();
    setLittleWill(false);
    chassis.moveToPoint(-33.125, 27, 1000, {.forwards = false});
}