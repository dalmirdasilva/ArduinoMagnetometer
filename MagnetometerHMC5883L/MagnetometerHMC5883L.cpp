#include "MagnetometerHMC5883L.h"
#include <inttypes.h>

MagnetometerHMC5883L::MagnetometerHMC5883L()
        : RegisterBasedWiredDevice(MAGNETOMETER_HMC5883L_DEVICE_ADDRESS) {
}

MagnetometerHMC5883L::~MagnetometerHMC5883L() {
}

double MagnetometerHMC5883L::getHeading() {
    unsigned char buf[6];
    int16_t x = 0, y = 0;
    readSample(buf);
    x = (buf[0] << 8) | buf[1];
    y = (buf[2] << 8) | buf[3];
    return computeVectorAngle(x, y);
}

void MagnetometerHMC5883L::setOperatingMode(OperatingMode om) {
    writeRegister(MagnetometerHMC5883L::MR, om & MAGNETOMETER_HMC5883L_MR_MASK);
}

void MagnetometerHMC5883L::setSamplesAveraged(SamplesAveraged sa) {
    configureRegisterBits(CRA, MAGNETOMETER_HMC5883L_CRA_MS_MASK, ((unsigned char) sa) << 5);
}

void MagnetometerHMC5883L::setDataOutputRate(DataOutputRate dar) {
    configureRegisterBits(CRA, MAGNETOMETER_HMC5883L_CRA_DO_MASK, ((unsigned char) dar) << 2);
}

void MagnetometerHMC5883L::setMeasurementMode(MeasurementMode mm) {
    configureRegisterBits(CRA, MAGNETOMETER_HMC5883L_CRA_MA_MASK, (unsigned char) mm);
}

void MagnetometerHMC5883L::setGain(Gain g) {
    MagnetometerHMC5883L::CRBbits crb = {0};
    crb.GN = (unsigned char) g;
    writeRegister(MagnetometerHMC5883L::CRB, crb.value);
}

MagnetometerHMC5883L::SRbits MagnetometerHMC5883L::getStatusRegister() {
    MagnetometerHMC5883L::SRbits sr = {0};
    sr.value = readRegister(SR);
    return sr;
}

void MagnetometerHMC5883L::readSample(unsigned char buf[6]) {
    readRegisterBlock(DXRA, buf, 0x06);
}

