#include "main.h" // IWYU pragma: keep
#include "intake.hpp"
#include "descore.hpp"

pros::adi::DigitalOut wing('E', false);

bool wingToggle = false;
bool midDescoreToggle = false;

void setWing(bool extended) {
    wingToggle = extended;
    wing.set_value(extended);
}

void descoreControl() {

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        setWing(true);
    } else {
        setWing(false);
    }
}