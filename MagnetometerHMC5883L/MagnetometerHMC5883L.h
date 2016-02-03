/**
 * Arduino - MagnetometerHMC5883L driver
 *
 * Concrete implementation of HMC5883L magnetometer.
 *
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_MAGNETOMETER_HMC5883L_H__
#define __ARDUINO_DRIVER_MAGNETOMETER_HMC5883L_H__ 1

#include <Magnetometer.h>
#include <RegisterBasedWiredDevice.h>

class MagnetometerHMC5883L : public Magnetometer, public RegisterBasedWiredDevice {

    /*
     * 00 Configuration Register A Read/Write
     * 01 Configuration Register B Read/Write
     * 02 Mode Register Read/Write
     * 03 Data Output X MSB Register Read
     * 04 Data Output X LSB Register Read
     * 05 Data Output Z MSB Register Read
     * 06 Data Output Z LSB Register Read
     * 07 Data Output Y MSB Register Read
     * 08 Data Output Y LSB Register Read
     * 09 Status Register Read
     * 10 Identification Register A Read
     * 11 Identification Register B Read
     * 12 Identification Register C Read
     */
    enum Register {
        CONFIGURATION_A = 0x00,
        CONFIGURATION_B = 0x01,
        MODE = 0x02,
        OUTPUT_X_MSB = 0x03,
        OUTPUT_X_LSB = 0x04,
        OUTPUT_Y_MSB = 0x05,
        OUTPUT_Y_LSB = 0x06,
        OUTPUT_Z_MSB = 0x07,
        OUTPUT_Z_LSB = 0x08,
        STATUS = 0x09,
        IDENTIFICATION_A = 0x0a,
        IDENTIFICATION_B = 0x0b,
        IDENTIFICATION_C = 0x0c
    };

public:

    /**
     * Public constructor.
     */
    MagnetometerHMC5883L();

    /**
     * Virtual destructor
     */
    virtual ~MagnetometerHMC5883L();

    /**
     * Gets the heading in degree.
     */
    double getHeading();
};

#endif // __ARDUINO_DRIVER_MAGNETOMETER_HMC5883L_H__
