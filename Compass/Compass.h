/**
 * Arduino - Compass driver
 *
 * Interface for all Compass implementations.
 *
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_COMPASS_H__
#define __ARDUINO_DRIVER_COMPASS_H__ 1

class Compass {

public:
    virtual ~Compass();

    /**
     * Gets the heading in degree.
     */
    virtual double getHeading() = 0;
};

#endif // __ARDUINO_DRIVER_COMPASS_H__
