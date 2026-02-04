#pragma once
#include "main.h" // IWYU pragma: keep
#include "lemlib/api.hpp" // IWYU pragma: keep

extern pros::Distance leftDistance;
extern pros::Distance rightDistance;
extern pros::Distance frontDistance;

extern double leftDistanceOffset;
extern double rightDistanceOffset;
extern double frontDistanceOffset;

extern const int halfFieldIn;

extern lemlib::Chassis chassis;

void resetDistances();