#pragma once
#include "main.h" // IWYU pragma: keep

extern pros::adi::DigitalOut wing;
extern pros::adi::DigitalOut wing1;
extern pros::adi::DigitalOut midDescore;
extern bool wingToggle;
extern bool midDescoreToggle;

void setWing(bool extended);
void setMidDescore(bool extended);
void descoreControl();