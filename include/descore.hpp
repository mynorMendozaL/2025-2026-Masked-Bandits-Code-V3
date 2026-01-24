#pragma once
#include "main.h" // IWYU pragma: keep

extern pros::adi::DigitalOut wing;
extern bool wingToggle;

void setWing(bool extended);
void descoreControl();