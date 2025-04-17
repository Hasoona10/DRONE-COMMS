#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <RH_RF69.h>

// === RFM69 Config ===
#define RFM69_CS     8
#define RFM69_INT    3
#define RFM69_RST    4
#define RF69_FREQ    915.0

RH_RF69 rf69(RFM69_CS, RFM69_INT);
Adafruit_MPU6050 mpu;

// === Motor Pins ===
const int AIN1_1 = 9;
const int AIN2_1 = 10;
const int BIN1_1 = 11;
const int BIN2_1 = 12;
const int AIN1_2 = 5;
const int AIN2_2 = 6;
const int BIN1_2 = 13;
const int BIN2_2 = 1;

// === Global State ===
int currentThrottle = 180;
char lastMovement = 'S';

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("🛩️ Feather M0 - RECEIVER MODE");

  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW); delay(10);
  digitalWrite(RFM69_RST, HIGH); delay(10);
  digitalWrite(RFM69_RST, LOW); delay(10);

  if (!rf69.init()) {
    Serial.println("❌ RFM69 init failed!");
    while (1);
  }

  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("❌ setFrequency failed");
  }

  rf69.setTxPower(14, true);
  Serial.println("✅ RFM69 listening on 915 MHz");

  pinMode(AIN1_1, OUTPUT); pinMode(AIN2_1, OUTPUT);
  pinMode(BIN1_1, OUTPUT); pinMode(BIN2_1, OUTPUT);
  pinMode(AIN1_2, OUTPUT); pinMode(AIN2_2, OUTPUT);
  pinMode(BIN1_2, OUTPUT); pinMode(BIN2_2, OUTPUT);
  stopAll();

  if (!mpu.begin()) {
    Serial.println("❌ MPU6050 not found");
  } else {
    Serial.println("✅ MPU6050 ready");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }
}

void loop() {
  if (rf69.available()) {
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf69.recv(buf, &len)) {
      if (len == 1) handleMovement(buf[0]);
      else if (len == 2 && buf[0] == 'T') handleThrottle(buf[1]);
      else if (len == 2) handleDiagonal(buf[0], buf[1]);
    }
  }
}

// === Movement Logic ===
void handleThrottle(uint8_t val) {
  currentThrottle = val;
  if (currentThrottle == 0) stopAll();
  else handleMovement(lastMovement);
}

void handleMovement(char cmd) {
  if (cmd != 'S') lastMovement = cmd;

  switch (cmd) {
    case 'F': moveForward(); break;
    case 'B': moveBackward(); break;
    case 'L': turnLeft(); break;
    case 'R': turnRight(); break;
    case 'S': stopAll(); break;
    default: Serial.println("⚠️ Unknown move"); break;
  }
}

void handleDiagonal(char a, char b) {
  if ((a == 'F' && b == 'L') || (a == 'L' && b == 'F')) moveForwardLeft();
  else if ((a == 'F' && b == 'R') || (a == 'R' && b == 'F')) moveForwardRight();
  else if ((a == 'B' && b == 'L') || (a == 'L' && b == 'B')) moveBackwardLeft();
  else if ((a == 'B' && b == 'R') || (a == 'R' && b == 'B')) moveBackwardRight();
  else Serial.println("⚠️ Unknown diagonal combo");
}

// === Motor Control ===
void moveForward() {
  analogWrite(AIN1_1, currentThrottle); digitalWrite(AIN2_1, LOW);
  analogWrite(BIN1_1, currentThrottle); digitalWrite(BIN2_1, LOW);
  analogWrite(AIN1_2, currentThrottle); digitalWrite(AIN2_2, LOW);
  analogWrite(BIN1_2, currentThrottle); digitalWrite(BIN2_2, LOW);
}

void moveBackward() {
  digitalWrite(AIN1_1, LOW); analogWrite(AIN2_1, currentThrottle);
  digitalWrite(BIN1_1, LOW); analogWrite(BIN2_1, currentThrottle);
  digitalWrite(AIN1_2, LOW); analogWrite(AIN2_2, currentThrottle);
  digitalWrite(BIN1_2, LOW); analogWrite(BIN2_2, currentThrottle);
}

void turnLeft() {
  digitalWrite(AIN1_1, LOW); analogWrite(AIN2_1, currentThrottle);
  digitalWrite(BIN1_1, LOW); analogWrite(BIN2_1, currentThrottle);
  analogWrite(AIN1_2, currentThrottle); digitalWrite(AIN2_2, LOW);
  analogWrite(BIN1_2, currentThrottle); digitalWrite(BIN2_2, LOW);
}

void turnRight() {
  analogWrite(AIN1_1, currentThrottle); digitalWrite(AIN2_1, LOW);
  analogWrite(BIN1_1, currentThrottle); digitalWrite(BIN2_1, LOW);
  digitalWrite(AIN1_2, LOW); analogWrite(AIN2_2, currentThrottle);
  digitalWrite(BIN1_2, LOW); analogWrite(BIN2_2, currentThrottle);
}

void moveForwardLeft() {
  analogWrite(AIN1_1, currentThrottle / 2); digitalWrite(AIN2_1, LOW);
  analogWrite(BIN1_1, currentThrottle); digitalWrite(BIN2_1, LOW);
  analogWrite(AIN1_2, currentThrottle); digitalWrite(AIN2_2, LOW);
  analogWrite(BIN1_2, currentThrottle / 2); digitalWrite(BIN2_2, LOW);
}

void moveForwardRight() {
  analogWrite(AIN1_1, currentThrottle); digitalWrite(AIN2_1, LOW);
  analogWrite(BIN1_1, currentThrottle / 2); digitalWrite(BIN2_1, LOW);
  analogWrite(AIN1_2, currentThrottle / 2); digitalWrite(AIN2_2, LOW);
  analogWrite(BIN1_2, currentThrottle); digitalWrite(BIN2_2, LOW);
}

void moveBackwardLeft() {
  digitalWrite(AIN1_1, LOW); analogWrite(AIN2_1, currentThrottle / 2);
  digitalWrite(BIN1_1, LOW); analogWrite(BIN2_1, currentThrottle);
  digitalWrite(AIN1_2, LOW); analogWrite(AIN2_2, currentThrottle);
  digitalWrite(BIN1_2, LOW); analogWrite(BIN2_2, currentThrottle / 2);
}

void moveBackwardRight() {
  digitalWrite(AIN1_1, LOW); analogWrite(AIN2_1, currentThrottle);
  digitalWrite(BIN1_1, LOW); analogWrite(BIN2_1, currentThrottle / 2);
  digitalWrite(AIN1_2, LOW); analogWrite(AIN2_2, currentThrottle / 2);
  digitalWrite(BIN1_2, LOW); analogWrite(BIN2_2, currentThrottle);
}

void stopAll() {
  analogWrite(AIN1_1, 0); analogWrite(AIN2_1, 0);
  analogWrite(BIN1_1, 0); analogWrite(BIN2_1, 0);
  analogWrite(AIN1_2, 0); analogWrite(AIN2_2, 0);
  analogWrite(BIN1_2, 0); analogWrite(BIN2_2, 0);
  Serial.println("⏹️ Motors stopped");
}
