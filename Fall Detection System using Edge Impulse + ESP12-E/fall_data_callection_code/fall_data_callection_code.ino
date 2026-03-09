#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Booting...");
  Wire.begin(4, 5);
  delay(100);
  mpu.initialize();
  delay(100);
  if (mpu.testConnection()) {
    Serial.println("MPU6050 OK");
  } else {
    Serial.println("MPU6050 FAILED - check wiring");
  }
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  Serial.print(ax / 16384.0);
  Serial.print(",");
  Serial.print(ay / 16384.0);
  Serial.print(",");
  Serial.println(az / 16384.0);

  delay(10); // 100Hz sampling
}