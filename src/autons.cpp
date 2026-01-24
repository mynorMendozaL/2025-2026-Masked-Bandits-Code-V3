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
    //Move towards Match Loader
    chassis.moveToPoint(35.3, 0, 1000);//35.2
    pros::delay(50);
    setLittleWill(true);
    chassis.turnToHeading(176, 750, {.maxSpeed = 90});

    //Move to Match Loader and intake
    chassis.moveToPoint(36.3, -34, 1000, {.maxSpeed = 70});//35.6
    intakeStore(127);
    pros::delay(975);

    //Move backwards to Long Goal and score
    chassis.moveToPoint(37.8, 22, 1000, {.forwards = false, .maxSpeed = 65}, false);
    outtakeLong(127);
    pros::delay(1200);
    chassis.moveToPoint(37.5, -7, 1000);
    pros::delay(400);
    intakeStop();
    setLittleWill(false);
}

void SAWP() {
    //Setting current robot pose to (0,0) facing 90 degrees
    chassis.setPose(0, 0, 90);

    //Move towards Match Loader
    chassis.moveToPoint(34.6, 0, 1000);
    pros::delay(50);
    setLittleWill(true);
    chassis.turnToHeading(176, 750, {.maxSpeed = 90});

    //Move to Match Loader and intake
    chassis.moveToPoint(35.75, -38, 1000, {.maxSpeed = 70});
    intakeStore(127);
    pros::delay(965);

    //Move backwards to Long Goal and score
    chassis.moveToPoint(37, 20, 1000, {.forwards = false, .maxSpeed = 60}, false);
    outtakeLong(127);
    pros::delay(1250);
    chassis.moveToPoint(37.5, -2, 1000);
    pros::delay(400);
    intakeStop();

    //Move towards 3 Block stack and intake
    chassis.turnToHeading(314, 750, {.direction = lemlib::AngularDirection::CW_CLOCKWISE});
    setLittleWill(false);
    chassis.moveToPoint(12, 11.5, 1000, {.maxSpeed = 60});
    bottomIntake.move(127);
    pros::delay(800);
    setLittleWill(true);
    pros::delay(700);

    //Move towards Lower Middle Goal and score
    chassis.moveToPoint(0, 23.25, 1000, {.maxSpeed =  60});// X -2, y 23
    pros::delay(100);
    setLittleWill(false);
    pros::delay(350);
    outtake(450);
    pros::delay(1700);

    //Move towards 2nd 3 Block stake and intake
    chassis.moveToPoint(15, 12.0, 1000, {.forwards = false});
    intakeStop();
    chassis.moveToPoint(-36, 12.0, 10000);
    intakeStore(127);
    pros::delay(900);
    setLittleWill(true);
    pros::delay(100);

    //Move towards Upper Middle Goal and score
    chassis.moveToPoint(-14.0, 29.25, 1000, {.forwards = false, .maxSpeed = 80}, false);
    outtakeUpperMid(108 );
    pros::delay(1750);
    intakeStop();
    setLittleWill(false);
    chassis.moveToPoint(-37, 7, 1000);
}

void right7Block() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(12.9, 26.75, 57, 1200, {.lead = 0.5}); //was moveToPoint
    intakeStore(127);
    pros::delay(900);
    setLittleWill(true);
    pros::delay(100);
    chassis.moveToPose(34, -24, 180, 1900, {.lead = 0.5});
    chassis.moveToPoint(34.286, 22, 1000, {.forwards = false, .maxSpeed = 70}, false);
    outtakeLong(127);
    pros::delay(1700);
    chassis.moveToPoint(24.15, 0, 1000);
    pros::delay(300);
    intakeStop();
    setLittleWill(false);
    wing.set_value(true);
    chassis.turnToHeading(180, 1000);
    pros::delay(200);
    wing.set_value(false);
    chassis.moveToPoint(24, 32, 1000, {.forwards = false, .maxSpeed = 80});
}

void left4Block() {
    //Setting current robot pose to (0,0) facing 90 degrees
    chassis.setPose(0, 0, -90);

    //Move towards Match Loader
    chassis.moveToPoint(-32.55, 0, 1000);//35.2
    pros::delay(50);
    setLittleWill(true);
    chassis.turnToHeading(-176, 750, {.maxSpeed = 90});

    //Move to Match Loader and intake
    chassis.moveToPoint(-34.75, -34, 1000, {.maxSpeed = 70});//35.6
    intakeStore(127);
    pros::delay(975);

    //Move backwards to Long Goal and score
    chassis.moveToPoint(-34, 22, 1000, {.forwards = false, .maxSpeed = 65}, false);
    outtakeLong(127);
    pros::delay(1200);
    chassis.moveToPoint(-35, -7, 1000);
    pros::delay(400);
    intakeStop();
    setLittleWill(false);
    
    //Extend Wing then move to use Wing on Goal
    wing.set_value(true);
    chassis.moveToPoint(-43.4, 0, 1000, {.forwards = false});
    chassis.turnToHeading(-182, 1000);
    chassis.moveToPoint(-44.1, 30.5, 1000, {.forwards = false, .maxSpeed = 60});
    wing.set_value(false);
}

void left7Block() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(-12.796, 26.75, -59, 1100, {.lead = 0.5}); //was moveToPoint
    intakeStore(127);
    pros::delay(800);
    setLittleWill(true);
    pros::delay(100);
    chassis.moveToPose(-33, -32, 180, 2050, {.lead = 0.5});
    chassis.moveToPoint(-31.6, 27, 1000, {.forwards = false, .maxSpeed = 60}, false);
    outtakeLong(127);
    pros::delay(1800);
    chassis.moveToPoint(-20.45, 0, 1000);
    pros::delay(350);
    intakeStop();
    setLittleWill(false);
}

void skillsAuton() {

}

void right43Block() {
    //Setting current robot pose to (0,0) facing 90 degrees
    chassis.setPose(0, 0, 90);

    //Move towards Match Loader
    chassis.moveToPoint(34.6, 0, 1000);
    pros::delay(50);
    setLittleWill(true);
    chassis.turnToHeading(176, 750, {.maxSpeed = 90});

    //Move to Match Loader and intake
    chassis.moveToPoint(35.75, -38, 1000, {.maxSpeed = 70});
    intakeStore(127);
    pros::delay(965);

    //Move backwards to Long Goal and score
    chassis.moveToPoint(37, 20, 1000, {.forwards = false, .maxSpeed = 60}, false);
    outtakeLong(127);
    pros::delay(1250);
    chassis.moveToPoint(37.5, -2, 1000);
    pros::delay(400);
    intakeStop();

    //Move towards 3 Block stack and intake
    chassis.turnToHeading(314, 750, {.direction = lemlib::AngularDirection::CW_CLOCKWISE});
    setLittleWill(false);
    chassis.moveToPoint(12, 11.5, 1000, {.maxSpeed = 60});
    bottomIntake.move(127);
    pros::delay(800);
    setLittleWill(true);
    pros::delay(700);

    //Move towards Lower Middle Goal and score
    chassis.moveToPoint(0, 23.25, 1000, {.maxSpeed =  60});// X -2, y 23
    pros::delay(100);
    setLittleWill(false);
    pros::delay(350);
    outtake(450);
    pros::delay(1700);

    //Move towards 2nd 3 Block stake and intake
    chassis.moveToPoint(15, 12.0, 1000, {.forwards = false});
    intakeStop();
}

void left43Block() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(-12.796, 26.75, -57, 1100, {.lead = 0.5}); //was moveToPoint
    intakeStore(127);
    pros::delay(800);
    setLittleWill(true);
    pros::delay(100);
    chassis.turnToHeading(-135, 1000);
    chassis.moveToPoint(7.25, 36.5, 1000, {.forwards = false}, false);
    outtakeUpperMid(85);
    pros::delay(1300);
    intakeStop();
    chassis.moveToPose(-34, -56, 180, 2200, {.lead = 0.72, .maxSpeed = 75});
    intakeStore(127);
    chassis.moveToPoint(-31.6, 22.3, 1000, {.forwards = false, .maxSpeed = 70}, false);
    outtakeLong(127);
    pros::delay(1800);
    chassis.moveToPoint(-20.55, 0, 1000);
    pros::delay(350);
    intakeStop();
    setLittleWill(false);
}