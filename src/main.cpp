#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "intake.hpp"
#include "littleWill.hpp"
#include "descore.hpp"

//left motor group
pros::MotorGroup left_motor_group ({-1, -12, -11}, pros::MotorGears::blue);
//right motor group
pros::MotorGroup right_motor_group ({13, 15, 14}, pros::MotorGears::blue);

// drivetrain settings
lemlib::Drivetrain drivetrain(&left_motor_group, // left motor group
                              &right_motor_group, // right motor group
                              12.2, // 12.3 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              450, // drivetrain rpm is 450
                              2 // horizontal drift is 2
);

// imu
pros::Imu imu(16);

// odometry settings
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(5, //joystick deadzone
									  10, //minimum output where drivetrain will move
									  1.02 //expo curve gain
);

//input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(5, //joystick deadzone
								   10, //minimum output where drivetrain will move
								   1.01 //expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors, // odometry sensors
						&throttle_curve, // throttle input curve
						&steer_curve // steer input curve
);

int autonSelection = 0;
bool autonStarted = false;

pros::adi::DigitalIn bumper('H');

void centerButton() {
	static bool pressed = false; // flag to check if right button was pressed
	pressed = !pressed; // toggle pressed state
	if (pressed) {
		outtakeLongMode++;
	} else if (outtakeLongMode >= 2) {
		outtakeLongMode = 0; // wrap around to 0 if above 2
	}
}


void initialize() {
	pros::lcd::initialize();
	chassis.calibrate();
	chassis.setPose(0, 0, 0); // set initial pose to (0,0,0)
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD); // set brake mode to hold
	bottomIntake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); // set brake mode to coast
	middleIntake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); // set brake mode to coast
	indexer.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); // set brake mode to coast
	intakeOptical.set_led_pwm(100); // set optical LED to full brightness
	intakeOptical.set_integration_time(10); // set optical integration time to 10ms
	pros::lcd::register_btn0_cb(centerButton);
}


void disabled() {

}


void competition_initialize() {
	while (!autonStarted) {
		if (bumper.get_value() == 1) {
			autonSelection++;
			pros::delay(500);
		} else if (autonSelection > 4) {
			autonSelection = 0;
		}
		
		switch(autonSelection) {
			case 0:
				pros::lcd::set_text(2, "red auton");
				break;
			case 1:
				pros::lcd::set_text(2, "blue auton");
				break;
			case 2:
				pros::lcd::set_text(2, "skills auton");
				break;
			case 3:
				pros::lcd::set_text(2, "auton disabled");
				break;
		}
		switch(outtakeLongMode) {
			case 0:
				pros::lcd::set_text(3, "Color Sort None");
				break;
			case 1:
				pros::lcd::set_text(3, "Color Sort Red");
				break;
			case 2:
				pros::lcd::set_text(3, "Color Sort Blue");
				break;
		}
	}
}


void autonomous() {
	autonStarted = true;
	if (autonSelection == 0) {
		
	} else if (autonSelection == 1) {
		
	} else if (autonSelection == 2) {
		
	}

}


void opcontrol() {

	while (true) {
	//Controller input for driving
		////////////////////////////////////////////////////////////
		int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y); // left joystick Y
		int rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // right joystick X
		chassis.arcade(leftY, rightX); // drive the robot with the joysticks using arcade control
		// Intake Control
		//////////////////////////////////////////////////////////////
		intakeControl();
		//////////////////////////////////////////////////////////////
		// Little Will Control
		littleWillControl();
		//////////////////////////////////////////////////////////////
		// Descore Control
		descoreControl();
		//////////////////////////////////////////////////////////////

	 pros::delay(20);                               // Run for 20 ms then update
	}
}