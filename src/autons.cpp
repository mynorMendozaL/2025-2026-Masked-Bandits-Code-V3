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
    chassis.moveToPoint(34.6, .25, 1000);
    pros::delay(50);
    chassis.turnToHeading(176, 500, {.maxSpeed = 90});//750
    pros::delay(100);
    setLittleWill(true);

    //Move to Match Loader and intake
    chassis.moveToPoint(34.6, -39.6, 1000, {.maxSpeed = 70});
    intakeStore(127);
    pros::delay(1090);

    //Move backwards to Long Goal and score
    chassis.moveToPoint(38, 20, 1000, {.forwards = false, .maxSpeed = 65}, false);
    outtakeLong(127);
    pros::delay(900);
    chassis.setPose(0,0,chassis.getPose().theta);
    pros::delay(5);
    chassis.swingToPoint(-28, 1.4, DriveSide::RIGHT, 750, {.minSpeed = 20, .earlyExitRange = 4});
    pros::delay(100);
    setLittleWill(false);
    /*chassis.moveToPoint(-0.55, -20, 1000);
    pros::delay(400);
    intakeStop();

    //Move towards 3 Block stack and intake
    chassis.turnToPoint(-27, 2.15, 750, {.direction = lemlib::AngularDirection::CW_CLOCKWISE, .minSpeed = 20, .earlyExitRange = 6});
    setLittleWill(false); */
    chassis.moveToPoint(-28, 1.4, 1000);
    intakeStore(100);
    pros::delay(475);//625
    setLittleWill(true);
    chassis.turnToPoint(-73, -6, 1000, {.minSpeed = 25, .earlyExitRange = 6});//-4
    chassis.moveToPoint(-73, -6, 10000);
    setLittleWill(false);
    pros::delay(865);
    setLittleWill(true);
    pros::delay(100);

    //Move towards Upper Middle Goal and score
    chassis.moveToPoint(-49, 20, 1000, {.forwards = false, .maxSpeed = 80}, false);//-50.5
    outtakeMid(127, 850);
    chassis.moveToPose(-93, -50, 180, 2300, {.lead = 0.45, .maxSpeed = 78});
    intakeStore(127);
    chassis.moveToPoint(-95.5, 0, 1000, {.forwards = false, .maxSpeed = 80}, false);
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
    pros::delay(775);
    setLittleWill(true);
    pros::delay(100);
    chassis.moveToPose(-36, -32, 180, 1905, {.lead = 0.5, . maxSpeed = 80});
    chassis.moveToPoint(-33.125, 27, 1000, {.forwards = false, .maxSpeed = 60}, false);
    outtake(127);
    pros::delay(150);
    outtakeLong(127);
    pros::delay(1700);
    chassis.moveToPoint(-22.5, 0, 1000);
    pros::delay(350);
    intakeStop();
    setLittleWill(false);
    chassis.turnToHeading(180, 600);
    chassis.moveToPoint(-22.5, 32.25, 1000, {.forwards = false, .maxSpeed = 80});
}

void skillsAuton() {
    //Setting current robot pose to (0,0) facing 90 degrees
    chassis.setPose(0, 0, 90);

    //Move towards Match Loader and load
    chassis.moveToPoint(34.531, 0.245, 5000);
    setLittleWill(true);
    chassis.turnToPoint(34.286, -10.041, 5000);
    intakeStore(127);
    chassis.moveToPoint(34.286, -10.041, 5000, {.maxSpeed = 80});
    pros::delay(2000);

    //Move towards other side of Long Goal
    chassis.moveToPoint(48.001, 7.837, 5000, {.forwards = false});
    chassis.turnToPoint(47.756, 81.797, 5000, {.forwards = false});
    chassis.moveToPoint(47.756, 81.797, 5000, {.forwards = false});
    chassis.turnToPoint(34.041, 90.369, 5000, {.forwards = false});
    chassis.moveToPoint(34.041, 90.369, 5000, {.forwards = false});
    chassis.turnToPoint(34.041, 77.879, 5000, {.forwards = false});
    chassis.moveToPoint(34.041, 77.879, 5000, {.forwards = false, .maxSpeed = 80}, false);
    outtakeLong(127);
    pros::delay(2000);
    chassis.moveToPoint(34.041, 103.838, 5000, {.maxSpeed = 80});
    chassis.moveToPoint(33.796, 77.879, 5000, {.forwards = false, .maxSpeed = 80}, false);
    outtakeLong(127);
    pros::delay(2000);
    chassis.moveToPoint(24.49, 88.899, 5000);
    setLittleWill(false);
    intakeStop();
    chassis.turnToPoint(10.776, 70.777, 5000);
    intakeStore(127);
    chassis.moveToPoint(10.776, 70.777, 5000, {.maxSpeed = 90});
    pros::delay(800);
    setLittleWill(true);
    chassis.turnToPoint(-36.98, 71.021, 5000);
    chassis.moveToPoint(-36.98, 71.021, 5000, {.maxSpeed = 90});
    pros::delay(300);
    setLittleWill(false);
    pros::delay(400);
    setLittleWill(true);
    chassis.turnToPoint(-23.511, 56.817, 5000);
    chassis.moveToPoint(-23.511, 56.817, 5000, {.forwards = true}, false);
    outtake(127);
    chassis.moveToPoint(-59.511, 87.43, 5000);
    chassis.moveToPoint(-59.511, 104.818, 5000);
    chassis.moveToPoint(-74.205, 73.96, 5000);
    chassis.moveToPoint(-73.715, 13.714, 5000);
    chassis.moveToPoint(-60.001, -0.49, 5000);
    chassis.moveToPoint(-60.246, 17.143, 5000);
    chassis.moveToPoint(-60.001, -12.49, 5000);
    chassis.moveToPoint(-60.491, 17.143, 5000);
    chassis.moveToPoint(-50.94, 0.735, 5000);
    chassis.moveToPoint(-37.225, 23.266, 5000);
    chassis.moveToPoint(10.286, 23.511, 5000);
    chassis.moveToPoint(-36.735, 23.266, 5000);
    chassis.moveToPoint(-27.184, 33.307, 5000);
    chassis.moveToPoint(-37.225, 23.266, 5000);
    chassis.moveToPoint(-13.225, 23.266, 5000);
    chassis.moveToPoint(-13.225, -18.368, 5000);
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
    outtakeMid(100, 1375);
    chassis.moveToPose(-34, -58.5, 180, 2300, {.lead = 0.72, .maxSpeed = 70});
    intakeStore(127);
    chassis.moveToPoint(-31.6, 22.3, 1000, {.forwards = false, .maxSpeed = 70}, false);
    outtakeLong(127);
    pros::delay(750);
    chassis.moveToPoint(-21, 0, 1000);
    pros::delay(350);
    intakeStop();
    setLittleWill(false);
    chassis.turnToHeading(183, 500);
    chassis.moveToPoint(-21, 30, 1000, {.forwards = false});
}