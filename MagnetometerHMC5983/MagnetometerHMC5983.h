/**
 * Arduino - MagnetometerHMC5983 driver
 *
 * Concrete implementation of HMC5983 magnetometer witch replaces HMC5883L one.
 *
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_MAGNETOMETER_HMC5983_H__
#define __ARDUINO_DRIVER_MAGNETOMETER_HMC5983_H__ 1

#include <MagnetometerHMC5883L.h>

#define MAGNETOMETER_HMC5983_MR_HS_MASK         0x80
#define MAGNETOMETER_HMC5983_MR_LP_MASK         0x20
#define MAGNETOMETER_HMC5983_MR_SIM_MASK        0x04
#define MAGNETOMETER_HMC5983_SR_DOW_MASK        0x10

/**
 * The same as MagnetometerHMC5883L but with temperature sensor.
 *
 * Temperature Compensation
 *
 * Temperature compensation of the measured magnetic data is enabled by default at the factory.
 * Temperature measured by the built-in temperature sensor will be used to compensate the
 * sensor’s sensitivity change due to temperature based on the sensor’s typical sensitivity
 * temperature coefficient. The compensated data will be placed in the Data Output Registers automatically.
 * Temperature sensor must be enabled (set CRA7 =1) for compensation to work.
 *
 * Temperature Output
 *
 * HMC5983 has a built-in temperature sensor that its output can be enabled by setting bit 7 of
 * Configuration Register A (CRA7). This bit is disabled at power-on by default.
 * When this feature is enabled, a temperature measurement will be taken at each magnetic measurement and
 * the output is placed in Temperature Output Registers (0x31 and 0x32).
 */
class MagnetometerHMC5983: public MagnetometerHMC5883L {

public:

    enum ExtendedRegister {
        TEMPH = 0x31,
        TEMPL = 0x32
    };

    /**
     * (Extended) Mode Register
     *
     * The mode register is an 8-bit register from which data can be read or to which data can be written.
     * This register is used to select the operating mode of the device. MR0 through MR7 indicate bit locations,
     * with MR denoting the bits that are in the mode register. MR7 denotes the first bit of the data stream.
     * The number in parenthesis indicates the default value of that bit. Mode register default is 0x01.
     *
     * MR7 to MR2 0:
     *      Bit MR7 is set to 1 internally after each single-measurement operation. Set to 0 when configuring mode register.
     *
     * MR1 to MR0 (MD1 to MD0):
     *      Mode Select Bits. These bits select the operation mode of this device.
     *
     *      MD1 MD0 -> Operating Mode
     *      00 -> Continuous-Measurement Mode. In continuous-measurement mode, the device continuously performs
     *          measurements and places the result in the data register. RDY goes high when new data is placed
     *          in all three registers. After a power-on or a write to the mode or configuration register,
     *          the first measurement set is available from all three data output registers after a period of 2/f DO and subsequent
     *          measurements are available at a frequency of f DO , where f DO is the frequency of data output.
     *      01 -> Single-Measurement Mode (Default). When single-measurement mode is selected, device performs
     *          a single measurement, sets RDY high and returned to idle mode. Mode register returns to idle mode bit values.
     *          The measurement remains in the data output register and RDY remains high until the data output
     *          register is read or another measurement is performed.
     *      10 -> Idle Mode. Device is placed in idle mode.
     *      11 -> Idle Mode. Device is placed in idle mode.
     */
    union EMRbits {

        struct {
            unsigned char MD0 :1;
            unsigned char MD1 :1;
            unsigned char SIM :1;
            unsigned char :2;
            unsigned char LP :1;
            unsigned char :1;
            unsigned char HS :1;
        };
        struct {
            unsigned char MD :2;
            unsigned char :6;
        };
        unsigned char value;
    };

    /**
     * (Extended) Status Register
     *
     * The status register is an 8-bit read-only register. This register is used to indicate device status. SR0 through SR7
     * indicate bit locations, with SR denoting the bits that are in the status register. SR7 denotes the first bit of the data stream.
     *
     * SR7 to SR5 0:
     *      These bits are reserved.
     *
     * SR4 (DOW):
     *      Data Over Written. Set when the measurement data are not read before the subsequent data measurements are
     *      posted to the output registers. This happens when master device skips reading one or more data samples.
     *      Bit is cleared at the beginning of a data read.
     *
     * SR3 to SR2 0:
     *      These bits are reserved.
     *
     * SR1 (LOCK):
     *      Data output register lock. This bit is set when:
     *      1. Some but not all for of the six data output registers have been read,
     *      2. Mode register has been read.
     *          When this bit is set, the six data output registers are locked and any new data will not be placed in
     *          these register until one of these conditions are met:
     *      1. All six bytes have been read, 2. the mode register is changed,
     *      3. The measurement configuration (CRA) is changed,
     *      4. Power is reset.
     *
     * SR0 (RDY):
     *      Ready Bit.
     *      Set when data is written to all six data registers.
     *      Cleared when device initiates a write to the data output registers and after one or more of the data output
     *      registers are written to. When RDY bit is clear it shall remain cleared for a 250 μs.
     *      DRDY pin can be used as an alternative to the status register for monitoring the device for measurement data.
     */
    union SRbits {

        struct {
            unsigned char RDY :1;
            unsigned char LOCK :1;
            unsigned char :2;
            unsigned char DOW :1;
            unsigned char :3;
        };
        unsigned char value;
    };

    /**
     * Speed mode.
     */
    enum SpeedMode {
        NORMAL_MODE = 0x00,
        HIGH_SPEED_MODE = 0x01
    };

    /**
     * Temperature sensor.
     */
    enum TemperatureSensor {
        DISABLE_TEMPERATURE_SENSOR = 0x00,
        ENABLE_TEMPERATURE_SENSOR = 0x01
    };

    /**
     * Lowest power mode.
     */
    enum LowestPowerMode {
        DISABLE_LOWEST_POWER_MODE = 0X00,
        ENABLE_LOWEST_POWER_MODE = 0X01
    };

    /**
     * SPI serial interface mode selection.
     */
    enum SerialInterfaceMode {
        FOUR_WIRE = 0X00,
        THREE_WIRE = 0X01
    };

    MagnetometerHMC5983();

    /**
     * Sets temperature sensor.
     *
     * CRA7 TS
     * Set this bit to enable temperature sensor.
     * Temperature sensor will be measured at each magnetic measurement.
     * Enable Temperature sensor for automatic compensation of
     * Sensitivity over temperature.
     *
     * @param temperatureSensor    TemperatureSensor option.
     */
    void setTemperatureSensor(unsigned char temperatureSensor);

    /**
     * Set speed mode.
     *
     * MR7 HS
     * Set this pin to enable I²C High Speed mode, 3400 kHz.
     *
     * @param speedMode    SpeedMode option.
     */
    void setHighSpeedMode(unsigned char speedMode);

    /**
     * Set Lowest power mode
     *
     * MR5 LP Lowest power mode. When set, ODR=0.75 Hz, and Averaging = 1.
     *
     * @param lowestPowerMode    LowestPowerMode option.
     */
    void setLowestPowerMode(unsigned char lowestPowerMode);

    /**
     * Set serial interface mode selection.
     *
     * SPI serial interface mode selection:
     *      0 -> 4-wire SPI interface
     *      1 -> 3-wire SPI interface
     *
     * @param serialInterfaceMode    SerialInterfaceMode option.
     */
    void setSerialInterfaceMode(unsigned char serialInterfaceMode);

    /**
     * Gets the temperature measurement.
     *
     * Temperature output in C is related to the temperature output register values as follows.
     * Temperature = (MSB * 2^8 + LSB) / (2^4 * 8) + 25 in C
     *
     * @return temperature in Celsius degrees.
     */
    double getTemperature();
};

#endif // __ARDUINO_DRIVER_MAGNETOMETER_HMC5983_H__
