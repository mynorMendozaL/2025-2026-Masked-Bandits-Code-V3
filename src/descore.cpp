#include "main.h" // IWYU pragma: keep
#include "intake.hpp"
#include "descore.hpp"

pros::adi::DigitalOut wing('D', false);
pros::adi::DigitalOut midDescore('E', false);

bool wingToggle = false;
bool midDescoreToggle = false;

void setWing(bool extended) {
    wingToggle = extended;
    wing.set_value(extended);
}

void setMidDescore(bool extended) {
    midDescoreToggle = extended;
    midDescore.set_value(extended);
}

void descoreControl() {
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        setWing(true);
    } else {
        setWing(false);
    }
    /*if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        setWing(!wingToggle);
        pros::delay(25);
    } */
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        setMidDescore(!midDescoreToggle);
        pros::delay(25);
    }
}