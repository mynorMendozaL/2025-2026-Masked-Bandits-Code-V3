#include "main.h" // IWYU pragma: keep
#include "distanceReset.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp" // IWYU pragma: keep
#include "lemlib/api.hpp" // IWYU pragma: keep
#include <cmath> // for acos, cos

extern lemlib::Chassis chassis; // declare chassis as extern

pros::Distance leftDistance(1);
pros::Distance rightDistance(2);
pros::Distance frontDistance(3);

double leftDistanceOffset = 0.0;
double rightDistanceOffset = 0.0;
double frontDistanceOffset = 0.0;

void resetDistances() {
    const double MIN_DIST = 50.0; // mm, minimum valid distance
    const double MAX_DIST = 4000.0; // mm, maximum valid distance

    double leftReading = leftDistance.get() - leftDistanceOffset;
    double rightReading = rightDistance.get() - rightDistanceOffset;
    double frontReading = frontDistance.get() - frontDistanceOffset;

    bool leftValid = leftReading >= MIN_DIST && leftReading <= MAX_DIST;
    bool rightValid = rightReading >= MIN_DIST && rightReading <= MAX_DIST;
    bool frontValid = frontReading >= MIN_DIST && frontReading <= MAX_DIST;

    // Convert valid readings to inches
    double leftDist = leftValid ? leftReading / 25.4 : 0.0;
    double rightDist = rightValid ? rightReading / 25.4 : 0.0;
    double frontDist = frontValid ? frontReading / 25.4 : 0.0;

    // Field width
    double fieldWidth = 144.0;

    lemlib::Pose currentPose = chassis.getPose();
    double currentHeading = currentPose.theta;

    

    /*if (leftValid && rightValid && frontValid) {
        // Full calculation using trigonometry
        double sumDist = leftDist + rightDist;
        double cosTheta = fieldWidth / sumDist;
        double theta = acos(cosTheta) * 180.0 / M_PI;
        if (leftDist <= rightDist) theta = -theta;
        double x = leftDist * cosTheta;
        double y = frontDist * cosTheta;
        chassis.setPose({(float)x, (float)y, (float)theta});
    } else if (leftValid && rightValid) {
        // Calculate theta and x using left and right sensors, keep current y
        double sumDist = leftDist + rightDist;
        double cosTheta = fieldWidth / sumDist;
        double theta = acos(cosTheta) * 180.0 / M_PI;
        if (leftDist <= rightDist) theta = -theta;
        double x = leftDist * cosTheta;
        chassis.setPose({(float)x, (float)currentPose.y, (float)theta});
    } else {
        // Partial position update without assuming heading
        double newX = currentPose.x;
        double newY = currentPose.y;
        double newTheta = currentPose.theta;

        if (leftValid) {
            newX = leftDist;
        }
        if (rightValid) {
            newX = fieldWidth - rightDist;
        }
        if (frontValid) {
            newY = frontDist;
        }

        chassis.setPose({(float)newX, (float)newY, (float)newTheta});
    }*/

    // No-trig reset: update position based on valid sensors, keep current heading
    double newX = currentPose.x;
    double newY = currentPose.y;

    if (leftValid) {
        newX = leftDist;
    } else if (rightValid) {
        newX = fieldWidth - rightDist;
    }

    if (frontValid) {
        newY = frontDist;
    }

    chassis.setPose({(float)newX, (float)newY, (float)currentPose.theta});
}