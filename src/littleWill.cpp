#include "main.h" // IWYU pragma: keep
#include "littleWill.hpp"
#include "intake.hpp"

pros::adi::DigitalOut littleWill('A', false);
bool littleWillToggle = false;

void setLittleWill(bool extended) {
    littleWillToggle = extended;
    littleWill.set_value(extended);
}
void littleWillControl() {
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
        setLittleWill(!littleWillToggle);
    }
}