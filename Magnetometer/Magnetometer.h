/**
 * Arduino - Magnetometer driver
 *
 * Interface for all Magnetometer (compass) implementations.
 *
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_MAGNETOMETER_H__
#define __ARDUINO_DRIVER_MAGNETOMETER_H__ 1

class Magnetometer {

public:

    virtual ~Magnetometer();

    /**
     * Gets the heading in degree.
     */
    virtual double getHeading() = 0;
};

#endif // __ARDUINO_DRIVER_MAGNETOMETER_H__
