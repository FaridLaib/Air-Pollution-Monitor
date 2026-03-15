#include <Wire.h>
#include <SensirionI2CSen5x.h>

SensirionI2CSen5x sen5x;

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Leonardo needs this — waits for USB serial to connect

  Wire.begin();
  sen5x.begin(Wire);

  uint16_t err = sen5x.deviceReset();
  if (err) { Serial.print("Reset error: "); Serial.println(err); return; }

  delay(1200);
  sen5x.setTemperatureOffsetSimple(0.0);

  err = sen5x.startMeasurement();
  if (err) { Serial.print("Start error: "); Serial.println(err); return; }

  Serial.println("SEN55 started — waiting 35s for PM warmup...");
  delay(35000);
  Serial.println("Ready. Reading every 2s:\n");
  Serial.println("Temp(C) | Temp(F) | Humidity | PM1.0 | PM2.5 | PM4.0 | PM10 | VOC | NOx");
  Serial.println("--------|---------|----------|-------|-------|-------|------|-----|----");
}

void loop() {
  float pm1, pm25, pm4, pm10, hum, temp, voc, nox;
  uint16_t err = sen5x.readMeasuredValues(pm1, pm25, pm4, pm10, hum, temp, voc, nox);

  if (err) {
    Serial.print("Read error: "); Serial.println(err);
  } else {
    int tempF = (int)(temp * 9.0 / 5.0 + 32.5);
    Serial.print(temp, 1);   Serial.print("      | ");
    Serial.print(tempF);     Serial.print("      | ");
    Serial.print(hum, 1);    Serial.print("     | ");
    Serial.print(pm1, 1);    Serial.print("  | ");
    Serial.print(pm25, 1);   Serial.print("  | ");
    Serial.print(pm4, 1);    Serial.print("  | ");
    Serial.print(pm10, 1);   Serial.print(" | ");
    Serial.print(voc, 0);    Serial.print("   | ");
    Serial.println(nox, 0);
  }

  delay(2000);
}
