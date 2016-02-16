#include "Magnetometer.h"
#include <Arduino.h>

Magnetometer::~Magnetometer() {
}

double Magnetometer::radiansToDegrees(double radians) {
    return radians * RAD_TO_DEG;
}

double Magnetometer::computeVectorAngle(int16_t x, int16_t y) {
    double degrees = radiansToDegrees(-atan2(y, x));
    if (degrees < 0) {
        degrees += 360.0;
    }
    if (degrees > 360.0) {
        degrees -= 360.0;
    }
    return degrees;
}
