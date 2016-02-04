#include "MagnetometerHMC5983.h"

MagnetometerHMC5983::MagnetometerHMC5983() {
}

void MagnetometerHMC5983::setTemperatureSensor(unsigned char temperatureSensor) {
    configureRegisterBits(CRA, 0x80, temperatureSensor << 7);
}

void MagnetometerHMC5983::setHighSpeedMode(unsigned char speedMode) {
    configureRegisterBits(MR, MAGNETOMETER_HMC5983_MR_HS_MASK, speedMode << 7);
}

void MagnetometerHMC5983::setLowestPowerMode(unsigned char lowestPowerMode) {
    configureRegisterBits(MR, MAGNETOMETER_HMC5983_MR_LP_MASK, lowestPowerMode << 5);
}

void MagnetometerHMC5983::setSerialInterfaceMode(unsigned char serialInterfaceMode) {
    configureRegisterBits(MR, MAGNETOMETER_HMC5983_MR_SIM_MASK, serialInterfaceMode << 3);
}

double MagnetometerHMC5983::getTemperature() {
    uint16_t raw = 0;
    readRegisterBlock(TEMPH, (unsigned char *) &raw, 2);
    return (raw / (double) 0x80) + 0x19;
}
