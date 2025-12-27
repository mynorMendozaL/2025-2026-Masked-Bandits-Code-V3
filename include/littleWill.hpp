#pragma once
#include "main.h" // IWYU pragma: keep

extern pros::adi::DigitalOut littleWill;
extern bool littleWillToggle;

void setLittleWill(bool extended);
void littleWillControl();