#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "intake.hpp"
#include "littleWill.hpp"
#include "descore.hpp"
#include "autons.hpp" // IWYU pragma: keep

//left motor group
pros::MotorGroup left_motor_group ({-1, -12, -11}, pros::MotorGears::blue);
//right motor group
pros::MotorGroup right_motor_group ({13, 15, 14}, pros::MotorGears::blue);

// drivetrain settings
lemlib::Drivetrain drivetrain(&left_motor_group, // left motor group
                              &right_motor_group, // right motor group
                              12, // 12 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              450, // drivetrain rpm is 450
                              8 // horizontal drift is 8
);

// imu
pros::Imu imu(16);

lemlib::TrackingWheel leftTrackingWheel(&left_motor_group, lemlib::Omniwheel::NEW_325, -6, 450);
lemlib::TrackingWheel rightTrackingWheel(&right_motor_group, lemlib::Omniwheel::NEW_325, 6, 450);

// odometry settings
lemlib::OdomSensors sensors(&leftTrackingWheel, // vertical tracking wheel 1, set to null
                            &rightTrackingWheel, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(5.8, // proportional gain (kP) //5.8
                                              0, // integral gain (kI)
                                              13, // derivative gain (kD) //13
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2.0, // proportional gain (kP) 1.9
                                              0, // integral gain (kI)
                                              13.6, // derivative gain (kD) 11.4
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
								   1.015 //expo curve gain
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

pros::adi::DigitalIn bumper('C');

void centerButton() {

}

void leftButton() {
	autonSelection++;
	if (autonSelection > 3) {
		autonSelection = 0; // wrap around to 0
	}
	pros::lcd::set_text(0, "Left pressed!");
}

void rightButton() {
	autonSelection--;
	if (autonSelection < 0) {
		autonSelection = 3; // wrap around to 3
	}
	pros::lcd::set_text(0, "Right pressed!");
}

void printInertialHeading() {
	while (true) {
		lemlib::Pose pose = chassis.getPose();
		double heading = imu.get_heading();
		std::string displayText = "X:" + std::to_string((int)pose.x) + 
		                          " Y:" + std::to_string((int)pose.y) + 
		                          " H:" + std::to_string((double)heading);
		master.set_text(0, 0, displayText);
		pros::delay(100); // update every 100ms
	}
}


void initialize() {
	pros::lcd::initialize();
	chassis.calibrate();
	chassis.setPose(0, 0, 0); // set initial pose to (0,0,0)
	//pros::lcd::register_btn0_cb(centerButton);
	//pros::lcd::register_btn1_cb(leftButton);
	//pros::lcd::register_btn2_cb(rightButton);
	chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD); // set brake mode to hold
	bottomIntake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); // set brake mode to coast
	indexer.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); // set brake mode to coast
\
	autonSelection = 0;
	autonStarted = false;
	
	//pros::Task printInertialTask(printInertialHeading);
}


void disabled() {

}


void competition_initialize() {
	// Ensure LCD is initialized
	pros::lcd::initialize();
	
	//pros::lcd::set_text(1, "Press buttons to select");
	
	// Track previous button states to detect presses
	uint8_t lastButtons = 0;

	while (!autonStarted) {
		// Read current button states
		uint8_t buttons = pros::lcd::read_buttons();
		
		// Detect button presses (button is pressed now but wasn't before)
		if ((buttons & LCD_BTN_LEFT) && !(lastButtons & LCD_BTN_LEFT)) {
			// Left button pressed
			autonSelection++;
			if (autonSelection > 7) {
				autonSelection = 0;
			}
			//pros::lcd::set_text(0, "Left pressed!");
		}
		
		
		if ((buttons & LCD_BTN_RIGHT) && !(lastButtons & LCD_BTN_RIGHT)) {
			// Right button pressed
			autonSelection--;
			if (autonSelection < 0) {
				autonSelection = 7;
			}
			//pros::lcd::set_text(0, "Right pressed!");
		}
		
		lastButtons = buttons;
		
		// Update display based on current selections
		switch(autonSelection) {
			case 0:
				pros::lcd::set_text(2, "Skills Auton");
				break;
			case 1:
				pros::lcd::set_text(2, "SAWP");
				break;
			case 2:
				pros::lcd::set_text(2, "Right 4 Block");
				break;
			case 3:
				pros::lcd::set_text(2, "Right 7 Block");
				break;
			case 4:
				pros::lcd::set_text(2, "Left 4 Block");
				break;
			case 5: 
				pros::lcd::set_text(2, "Left 7 Block");
				break;
			case 6:
				pros::lcd::set_text(2, "Right 4+3 Block");
				break;
			case 7:
				pros::lcd::set_text(2, "Left 4+3 Block");
				break;
		}
		pros::delay(300); // 300ms delay for button debouncing
	}
}


void autonomous() {
/*	autonStarted = true;
	if (autonSelection == 0) {
		left43Block();
	} else if (autonSelection == 1) {
		SAWP();
	} else if (autonSelection == 2) {
		right4Block();
	} else if (autonSelection == 3) {
		right7Block();
	} else if (autonSelection == 4) {
		left4Block();
	} else if (autonSelection == 5) {
		left7Block();
	} else if (autonSelection ==6) {
		right43Block();
	} else if (autonSelection ==7) {
		left43Block();
	}
*/
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
		
	 pros::delay(10);                               // Run for 20 ms then update
	}
}