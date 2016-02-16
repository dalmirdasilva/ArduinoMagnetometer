#include <Wire.h>
#include <Magnetometer.h>
#include <WiredDevice.h>
#include <RegisterBasedWiredDevice.h>
#include <MagnetometerHMC5883L.h>

/**
 * Pinout
 * 
 * <pre>
 * Sensor   -> Arduino
 * -------------------
 * SCL      -> A5
 * SDA      -> A4
 * 
 * VCC      -> 3v3
 * GND      -> GND
 * </pre>
 */

MagnetometerHMC5883L mag;

void setup() {
    Serial.begin(9600);
    mag.setDataOutputRate(MagnetometerHMC5883L::DAR_1_5);
    mag.setOperatingMode(MagnetometerHMC5883L::CONTINUOUS_MEASUREMENT_MODE);
}

MagnetometerHMC5883L::SRbits sr;

void loop() {
    sr = mag.getStatusRegister();
    
    Serial.print("Status register: ");
    Serial.println(sr.value, BIN);

    Serial.print("heading: ");
    Serial.println(mag.getHeading());

    Serial.println("-----------\n\n");
    delay(1000);
}