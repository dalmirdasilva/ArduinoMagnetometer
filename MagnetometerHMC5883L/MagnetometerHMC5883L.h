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

#define MAGNETOMETER_HMC5883L_DEVICE_ADDRESS    0x1e

#define MAGNETOMETER_HMC5883L_CRA_MS_MASK       0x60
#define MAGNETOMETER_HMC5883L_CRA_DO_MASK       0x1c
#define MAGNETOMETER_HMC5883L_CRA_MA_MASK       0x03

#define MAGNETOMETER_HMC5883L_MR_MASK           0x03

/**
 * The Honeywell HMC5883L is a surface-mount, multi-chip module designed for
 * low-field magnetic sensing with a digital interface for applications such as low-cost
 * compassing and magnetometry. The HMC5883L includes our state-of-the-art,
 * high-resolution HMC118X series magneto-resistive sensors plus an ASIC
 * containing amplification, automatic degaussing strap drivers, offset cancellation,
 * and a 12-bit ADC that enables 1° to 2° compass heading accuracy. The I2C
 * serial bus allows for easy interface. The HMC5883L is a 3.0x3.0x0.9mm surface
 * mount 16-pin leadless chip carrier (LCC). Applications for the HMC5883L
 * include Mobile Phones, Netbooks, Consumer Electronics, Auto Navigation Systems,
 * and Personal Navigation Devices.
 */
class MagnetometerHMC5883L: public Magnetometer, public RegisterBasedWiredDevice {

public:

    /**
     * Configuration Register A
     *
     * CRA6 to CRA5 (MA1 to MA0):
     *      Select number of samples averaged (1 to 8) per measurement output.
     *
     *      MA1 MA0 ->Number of samples.
     *      00 -> 1 (Default);
     *      01 -> 2;
     *      10 -> 4;
     *      11 -> 8;
     *
     * CRA4 to CRA2 (DO2 to DO0):
     *      Data Output Rate Bits. These bits set the rate at which data is written
     *      to all three data output registers.
     *
     *      DO2 DO1 DO0 -> Typical Data Output Rate (Hz)
     *      000 -> 0.75
     *      001 -> 1.5
     *      010 -> 3
     *      011 -> 7.5
     *      100 -> 15 (Default)
     *      101 -> 30
     *      110 -> 75
     *      111 -> Reserved
     *
     * CRA1 to CRA0 (MS1 to MS0):
     *      Measurement Configuration Bits. These bits define the measurement
     *      flow of the device, specifically whether or not to incorporate an
     *      applied bias into the measurement.
     *
     *      MS1 MS0 -> Measurement Mode
     *      00 -> Normal measurement configuration (Default).
     *          In normal measurement configuration the device follows normal measurement flow.
     *          The positive and negative pins of the resistive load are left floating and high impedance.
     *      01 -> Positive bias configuration for X, Y, and Z axes. In this configuration, a positive
     *          current is forced across the resistive load for all three axes.
     *      10 -> Negative bias configuration for X, Y and Z axes. In this configuration, a negative
     *          current is forced across the resistive load for all three axes.
     *      11 -> This configuration is reserved.
     *
     * CRA default is 0x10
     */
    union CRAbits {

        struct {
            unsigned char MS0 :1;
            unsigned char MS1 :1;
            unsigned char DO0 :1;
            unsigned char DO1 :1;
            unsigned char DO2 :1;
            unsigned char MA0 :1;
            unsigned char MA1 :1;
            unsigned char :1;
        };
        struct {
            unsigned char MS :2;
            unsigned char DO :3;
            unsigned char MA :2;
            unsigned char :1;
        };
        unsigned char value;
    };

    /**
     * Configuration Register B
     *
     * The configuration register B for setting the device gain.
     * CRB0 through CRB7 indicate bit locations, with CRB denoting the bits that are in the configuration register.
     * CRB7 denotes the first bit of the data stream. The number in parenthesis indicates the default value of that bit.
     *
     * CRB7 to CRB5 (GN2 to GN0):
     *      Gain Configuration Bits. These bits configure the gain for the device.
     *      The gain configuration is common for all channels.
     *
     *      GN2 GN1 GN0 -> {Recommended Sensor Field Range} {Gain (LSb/Gauss)} {Digital Resolution (mG/LSb)} {Output Range}
     *      000 -> ±0.88Ga 1370 0.73 {0xF800–0x07FF (-2048–2047)}
     *      001 -> ±1.3Ga {1090 (default)} 0.92 {0xF800–0x07FF (-2048–2047)}
     *      010 -> ±1.9Ga 820 1.22 {0xF800–0x07FF (-2048–2047)}
     *      011 -> ±2.5Ga 660 1.52 {0xF800–0x07FF (-2048–2047)}
     *      100 -> ±4.0Ga 440 2.27 {0xF800–0x07FF (-2048–2047)}
     *      101 -> ±4.7Ga 390 2.56 {0xF800–0x07FF (-2048–2047)}
     *      110 -> ±5.6Ga 330 3.03 {0xF800–0x07FF (-2048–2047)}
     *      111 -> ±8.1Ga 230 4.35 {0xF800–0x07FF (-2048–2047)}
     *
     * CRB4 to CRB0 0:
     *      These bits must be cleared for correct operation.
     *
     * CRB default is 0x20.
     */
    union CRBbits {

        struct {
            unsigned char :5;
            unsigned char GN0 :1;
            unsigned char GN1 :1;
            unsigned char GN2 :1;
        };
        struct {
            unsigned char :5;
            unsigned char GN :3;
        };
        unsigned char value;
    };

    /**
     * Mode Register
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
    union MRbits {

        struct {
            unsigned char MD0 :1;
            unsigned char MD1 :1;
            unsigned char :6;
        };
        struct {
            unsigned char MD :2;
            unsigned char :6;
        };
        unsigned char value;
    };

    /**
     * Data Output X Registers A and B
     *
     * The data output X registers are two 8-bit registers, data output register A and data output register B. These registers
     * store the measurement result from channel X. Data output X register A contains the MSB from the measurement result,
     * and data output X register B contains the LSB from the measurement result. The value stored in these two registers is a
     * 16-bit value in 2’s complement form, whose range is 0xF800 to 0x07FF. DXRA0 through DXRA7 and DXRB0 through
     * DXRB7 indicate bit locations, with DXRA and DXRB denoting the bits that are in the data output X registers. DXRA7 and
     * DXRB7 denote the first bit of the data stream. The number in parenthesis indicates the default value of that bit.
     * In the event the ADC reading overflows or underflows for the given channel, or if there is a math overflow during the bias
     * measurement, this data register will contain the value -4096. This register value will clear when after the next valid measurement is made.
     *
     * In the event the ADC reading overflows or underflows for the given channel, or if there is a math overflow during the bias
     * measurement, this data register will contain the value -4096. This register value will clear when after the next valid
     * measurement is made.
     */

    /**
     * Data Output Y Registers A and B
     *
     * The data output Y registers are two 8-bit registers, data output register A and data output register B. These registers
     * store the measurement result from channel Y. Data output Y register A contains the MSB from the measurement result,
     * and data output Y register B contains the LSB from the measurement result. The value stored in these two registers is a
     * 16-bit value in 2’s complement form, whose range is 0xF800 to 0x07FF. DYRA0 through DYRA7 and DYRB0 through
     * DYRB7 indicate bit locations, with DYRA and DYRB denoting the bits that are in the data output Y registers. DYRA7 and
     * DYRB7 denote the first bit of the data stream. The number in parenthesis indicates the default value of that bit.
     *
     * In the event the ADC reading overflows or underflows for the given channel, or if there is a math overflow during the bias
     * measurement, this data register will contain the value -4096. This register value will clear when after the next valid
     * measurement is made.
     */

    /**
     * Data Output Z Registers A and B
     *
     * The data output Z registers are two 8-bit registers, data output register A and data output register B. These registers
     * store the measurement result from channel Z. Data output Z register A contains the MSB from the measurement result,
     * and data output Z register B contains the LSB from the measurement result. The value stored in these two registers is a
     * 16-bit value in 2’s complement form, whose range is 0xF800 to 0x07FF. DZRA0 through DZRA7 and DZRB0 through
     * DZRB7 indicate bit locations, with DZRA and DZRB denoting the bits that are in the data output Z registers. DZRA7 and
     * DZRB7 denote the first bit of the data stream. The number in parenthesis indicates the default value of that bit.
     *
     * In the event the ADC reading overflows or underflows for the given channel, or if there is a math overflow during the bias
     * measurement, this data register will contain the value -4096. This register value will clear when after the next valid
     * measurement is made.
     */

    /**
     * Data Output Register Operation
     *
     * When one or more of the output registers are read, new data cannot be placed in any of the output data registers until all
     * six data output registers are read. This requirement also impacts DRDY and RDY, which cannot be cleared until new
     * data is placed in all the output registers.
     */

    /**
     * Status Register
     *
     * The status register is an 8-bit read-only register. This register is used to indicate device status. SR0 through SR7
     * indicate bit locations, with SR denoting the bits that are in the status register. SR7 denotes the first bit of the data stream.
     *
     * SR7 to SR2 0:
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
            unsigned char :6;
        };
        unsigned char value;
    };

    /**
     * Identification Register A
     *
     * The identification register A is used to identify the device. IRA0 through IRA7 indicate bit locations,
     * with IRA denoting the bits that are in the identification register A. IRA7 denotes the first bit of the data stream.
     * The number in parenthesis indicates the default value of that bit.
     * The identification value for this device is stored in this register. This is a read-only register.
     * Register values.
     *
     * ASCII value H
     */

    /**
     * Identification Register B
     *
     * The identification register B is used to identify the device. IRB0 through IRB7 indicate bit locations, with IRB denoting the
     * bits that are in the identification register A. IRB7 denotes the first bit of the data stream.
     * Register values.
     *
     * ASCII value 4
     */

    /**
     * Identification Register C
     *
     * The identification register C is used to identify the device. IRC0 through IRC7 indicate bit locations,
     * with IRC denoting the bits that are in the identification register A. IRC7 denotes the first bit of the data stream.
     * Register values.
     *
     * ASCII value 3
     */

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
        CRA = 0x00,
        CRB = 0x01,
        MR = 0x02,
        DXRA = 0x03,
        DXRB = 0x04,
        DYRA = 0x05,
        DYRB = 0x06,
        DZRA = 0x07,
        DZRB = 0x08,
        SR = 0x09,
        IDA = 0x0a,
        IDB = 0x0b,
        IDC = 0x0c
    };

    /**
     * Modes Of Operation
     *
     * Continuous-Measurement Mode
     *
     * During continuous-measurement mode, the device continuously makes measurements, at user selectable rate, and
     * places measured data in data output registers. Data can be re-read from the data output registers if necessary; however,
     * if the master does not ensure that the data register is accessed before the completion of the next measurement, the data
     * output registers are updated with the new measurement. To conserve current between measurements, the device is
     * placed in a state similar to idle mode, but the Mode Register is not changed to Idle Mode. That is, MD[n] bits are
     * unchanged. Settings in the Configuration Register A affect the data output rate (bits DO[n]), the measurement
     * configuration (bits MS[n]), when in continuous-measurement mode.
     * All registers maintain values while in continuous-measurement mode. The I2C bus is enabled for use by other devices
     * on the network in while continuous-measurement mode.
     *
     * Single-Measurement Mode
     *
     * This is the default power-up mode. During single-measurement mode, the device makes a single measurement and
     * places the measured data in data output registers. After the measurement is complete and output data registers are
     * updated, the device is placed in idle mode, and the Mode Register is changed to idle mode by setting MD[n] bits. Settings
     * in the configuration register affect the measurement configuration (bits MS[n])when in single-measurement mode.
     * All registers maintain values while in single-measurement mode. The I2C bus is enabled for use by other devices on the
     * network while in single-measurement mode.
     *
     * Idle Mode
     *
     * During this mode the device is accessible through the I2C bus, but major sources of power consumption are disabled,
     * such as, but not limited to, the ADC, the amplifier, and the sensor bias current. All registers maintain values while in idle mode.
     * The I2C bus is enabled for use by other devices on the network while in idle mode.
     */
    enum OperatingMode {
        IDLE_MODE = 0x00,
        CONTINUOUS_MEASUREMENT_MODE = 0x01,
        SINGLE_MEASUREMENT_MODE = 0x02
    };

    /**
     * Number of samples averaged (1 to 8) per measurement output.
     */
    enum SamplesAveraged {
        SA_1 = 0x00,
        SA_2 = 0x01,
        SA_4 = 0x02,
        SA_8 = 0x03,
    };

    /**
     * These bits set the rate at which data is written to all three data output registers.
     */
    enum DataOutputRate {
        DAR_0_75 = 0x00,
        DAR_1_5 = 0x01,
        DAR_3 = 0x02,
        DAR_7_5 = 0x03,
        DAR_15 = 0x04,
        DAR_30 = 0x05,
        DAR_75 = 0x06
    };

    /**
     * These bits define the measurement flow of the device,
     * specifically whether or not to incorporate an applied bias into the measurement.
     */
    enum MeasurementMode {
        NORMAL_MEASUREMENT = 0x00,
        POSITIVE_BIAS = 0x01,
        NEGATIVE_BIAS = 0x02
    };

    /**
     * These bits configure the gain for the device.
     * The gain configuration is common for all channels.
     */
    enum Gain {
        GAIN_0_88_GA = 0x00,
        GAIN_1_3_GA = 0x01,
        GAIN_1_9_GA = 0x02,
        GAIN_2_5_GA = 0x03,
        GAIN_4_0_GA = 0x04,
        GAIN_4_7_GA = 0x05,
        GAIN_5_6_GA = 0x06,
        GAIN_8_1_GA = 0x07
    };

    /**
     * Public constructor.
     *
     * The HMC5883L has a fairly quick stabilization time from no voltage to stable and ready for data retrieval.
     * The nominal 56 milli-seconds with the factory default single measurement mode means that the six bytes of magnetic data registers
     * (DXRA, DXRB, DZRA, DZRB, DYRA, and DYRB) are filled with a valid first measurement.
     */
    MagnetometerHMC5883L();

    /**
     * Configure operating mode.
     *
     * The mode register is an 8-bit register from which data can be read or to which data can be written.
     * This register is used to select the operating mode of the device. MR0 through MR7 indicate bit locations,
     * with MR denoting the bits that are in the mode register. MR7 denotes the first bit of the data stream.
     * The number in parenthesis indicates the default value of that bit.
     * Mode register default is 0x01.
     *
     * @param operatingMode    Operating Mode
     */
    void setOperatingMode(unsigned char operatingMode);

    /**
     * Virtual destructor
     */
    virtual ~MagnetometerHMC5883L();

    /**
     * Sets samples averaged.
     *
     * Select number of samples averaged (1 to 8) per measurement output.
     *
     * @param samplesAveraged    Number of samples averaged (1 to 8) per measurement output.
     */
    void setSamplesAveraged(unsigned char samplesAveraged);

    /**
     * Sets data output rate.
     *
     * These bits set the rate at which data is written to all three data output registers.
     *
     * @param dataOutputRate       Rate.
     */
    void setDataOutputRate(unsigned char dataOutputRate);

    /**
     * Set measurement mode.
     *
     * These bits define the measurement flow of the device,
     * specifically whether or not to incorporate an applied bias into the measurement.
     *
     * @param measurementMode    Measurement mode.
     */
    void setMeasurementMode(unsigned char measurementMode);

    /**
     * Sets gain.
     *
     * These bits configure the gain for the device.
     * The gain configuration is common for all channels.
     *
     * NOTE: Choose a lower gain value (higher GN#) when total field strength causes overflow in one of the
     * data output registers (saturation). Note that the very first measurement after a gain change
     * maintains the same gain as the previous setting.
     *
     * Register: CRB
     *
     * @param gain     Gain.
     */
    void setGain(unsigned char gain);

    /**
     * Gets the status register.
     *
     * The status register is an 8-bit read-only register. This register is used to indicate device status.
     * SR0 through SR7 indicate bit locations, with SR denoting the bits that are in the status register.
     * SR7 denotes the first bit of the data stream.
     */
    SRbits getStatusRegister();

    /**
     * Reads the sample.
     *
     * Read all 6 bytes. If gain is changed then this data set is using previous gain.
     */
    void readSample(unsigned char buf[6]);

    /**
     * Gets the heading in degree.
     */
    double getHeading();
};

#endif // __ARDUINO_DRIVER_MAGNETOMETER_HMC5883L_H__
