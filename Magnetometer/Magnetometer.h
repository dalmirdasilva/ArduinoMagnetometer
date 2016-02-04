/**
 * Arduino - Magnetometer driver
 *
 * Interface for all Magnetometer (compass) implementations.
 *
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_MAGNETOMETER_H__
#define __ARDUINO_DRIVER_MAGNETOMETER_H__ 1

#include <math.h>
#include <inttypes.h>

class Magnetometer {

public:

    virtual ~Magnetometer();

    /**
     * Gets the heading in degree.
     */
    virtual double getHeading() = 0;

    /**
     * Radians to degrees.
     *
     * @param radians   Radians.
     * @return          Degrees.
     */
    double inline radiansToDegrees(double radians);

    /**
     * To convert the micro-Tesla readings into a 0-360 degree compass heading,
     * we can use the atan2() function to compute the angle of the vector defined
     * by the Y and X axis readings. The result will be in radians, so we multiply
     * by 180 degrees and divide by Pi to convert that to degrees.
     *
     * @param x         X read in micro-tesla
     * @param y         Y read in micro-tesla
     * @return          The heading in degrees.
     */
    double computeVectorAngle(int16_t x, int16_t y);
};

#endif // __ARDUINO_DRIVER_MAGNETOMETER_H__
