#include "main.h" // IWYU pragma: keep
#include "intake.hpp"
#include "descore.hpp"

pros::adi::DigitalOut wing('D', false);
pros::adi::DigitalOut wing1('E', false);
pros::adi::DigitalOut midDescore('G', false);

bool wingToggle = false;
bool midDescoreToggle = false;

void setWing(bool extended) {
    wingToggle = extended;
    wing.set_value(extended);
    wing1.set_value(extended);
}

void setMidDescore(bool extended) {
    midDescoreToggle = extended;
    midDescore.set_value(extended);
}

void descoreControl() {

    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        setWing(!wingToggle);
    } 
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        setMidDescore(!midDescoreToggle);
    }
}