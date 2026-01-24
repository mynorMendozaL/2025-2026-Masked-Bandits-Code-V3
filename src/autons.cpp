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
    outtakeLongMode=1;

    //Move towards Match Loader
    chassis.moveToPoint(35.3, 0, 1000);//35.2
    pros::delay(50);
    setLittleWill(true);
    chassis.turnToHeading(176, 750, {.maxSpeed = 90});

    //Move to Match Loader and intake
    chassis.moveToPoint(36.3, -34, 1000, {.maxSpeed = 70});//35.6
    intakeStoreOnce(127);
    pros::delay(975);

    //Move backwards to Long Goal and score
    chassis.moveToPoint(37.8, 22, 1000, {.forwards = false, .maxSpeed = 65}, false);
    outtakeLong(127);
    pros::delay(1200);
    chassis.moveToPoint(37.5, -7, 1000);
    pros::delay(400);
    intakeStop();
    setLittleWill(false);
    
    //Extend Wing then move to use Wing on Goal
    //wing1.set_value(true);
    chassis.moveToPoint(48.3, 0, 1000, {.forwards = false});
    chassis.turnToHeading(182, 1000);
    chassis.moveToPoint(50, 28.5, 1000, {.forwards = false, .maxSpeed = 60});
    //wing1.set_value(false);
}

void SAWP() {
    //Setting current robot pose to (0,0) facing 90 degrees
    chassis.setPose(0, 0, 90);
    outtakeLongMode = 1;

    //Move towards Match Loader
    chassis.moveToPoint(34.9, 0, 1000);
    pros::delay(50);
    setLittleWill(true);
    chassis.turnToHeading(176, 750, {.maxSpeed = 90});

    //Move to Match Loader and intake
    chassis.moveToPoint(36.3, -34, 1000, {.maxSpeed = 70});
    intakeStoreOnce(127);
    pros::delay(965);

    //Move backwards to Long Goal and score
    chassis.moveToPoint(38.2, 20, 1000, {.forwards = false, .maxSpeed = 60}, false);
    outtakeLong(127);
    pros::delay(1250);
    chassis.moveToPoint(37.5, -2, 1000);
    pros::delay(400);
    intakeStop();

    //Move towards 3 Block stack and intake
    chassis.turnToHeading(309, 750, {.direction = lemlib::AngularDirection::CW_CLOCKWISE});
    setLittleWill(false);
    chassis.moveToPoint(12, 9.7, 1000, {.maxSpeed = 60});
    intakeStoreOnce(127);
    pros::delay(600);
    //setLittleWill(true);
    pros::delay(100);

    //Move towards Lower Middle Goal and score
    chassis.moveToPoint(-3, 22, 1000, {.maxSpeed =  80});
    //setLittleWill(false);
    pros::delay(250);
    outtakeLowerMid(127);
    pros::delay(1200);

    //Move towards 2nd 3 Block stake and intake
    chassis.moveToPoint(15, 7, 1000, {.forwards = false});
    intakeStop();
    chassis.moveToPoint(-36, 5, 10000);
    intakeStoreOnce(127);
    pros::delay(1000);
    setLittleWill(true);
    pros::delay(100);

    //Move towards Upper Middle Goal and score
    chassis.moveToPoint(-23, 12, 1000, {.forwards = false}, false);
    outtakeUpperMid(127);
    pros::delay(500);
    chassis.moveToPoint(-37, 7, 1000);
}

void right7Block() {
    chassis.setPose(0, 0, 0);
    /*chassis.moveToPoint(0.796, 16, 1000, {.minSpeed = 127, .earlyExitRange = 5});
    intakeStoreOnce(127);*/
    chassis.moveToPose(9.796, 24.735, 55, 3000, {.lead = 0.7}); //was moveToPoint
    pros::delay(800);
    setLittleWill(true);
    pros::delay(100);
    chassis.moveToPoint(35.021, -0.735, 1000);
    chassis.moveToPoint(34.776, -12.98, 1000);
    pros::delay(1000);
    chassis.moveToPoint(34.286, 17.143, 1000, {.forwards = false, .maxSpeed = 80}, false);
    outtakeLong(127);
    pros::delay(1500);
    chassis.moveToPoint(27.919, 0, 1000);
    intakeStop();
    setLittleWill(false);
    setWing(true);
    chassis.moveToPoint(23.266, 18.368, 1000, {.forwards = false});
    pros::delay(200);
    setWing(false);
    chassis.moveToPoint(23.266, 36.735, 1000, {.forwards = false});
}