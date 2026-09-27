/*
  Gate leakage measurement sketch

  Transcribed from the final LCD/Arduino code shown in the ES327 report
  (section 3.10, printed page 30). It is a report-based transcription,
  not a verified copy of the original firmware.

  The report's displayed sketch reads A0 and calculates current using a
  101 MOhm factor. Elsewhere, the report mentions A1 and describes other
  averaging/gain settings. Confirm the actual wiring and calibration before
  using this sketch with hardware.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

// I2C address and dimensions as shown in the report.
LiquidCrystal_I2C lcd(0x20, 16, 2);

#define NUM_SAMPLES 50

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Voltage & Current");
}

void loop() {
  float voltageSum = 0.0;

  // Read 50 samples and calculate the mean voltage.
  for (int i = 0; i < NUM_SAMPLES; i++) {
    int sensorValue = analogRead(A0);
    float voltage = sensorValue * (5.0 / 1023.0); // Assumes a 5 V reference.
    voltageSum += voltage;
    delay(5);
  }

  float voltageMean = voltageSum / NUM_SAMPLES;
  float currentNanoAmps = (voltageMean / (101.0 * pow(10, 6))) * pow(10, 9);

  // Report the mean values in the Serial Monitor.
  Serial.print("Mean Voltage: ");
  Serial.print(voltageMean, 3);
  Serial.print(" V, Mean Ig: ");
  Serial.print(currentNanoAmps, 3);
  Serial.println(" nA");

  // Display voltage and calculated current on the LCD.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("V: ");
  lcd.print(voltageMean, 2);
  lcd.print(" V");

  lcd.setCursor(0, 1);
  lcd.print("Ig: ");
  lcd.print(currentNanoAmps, 2);
  lcd.print(" nA");

  delay(1000);
}
