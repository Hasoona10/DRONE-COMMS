#include <SPI.h>
#include <RH_RF69.h>

// === RFM69 Config ===
#define RFM69_CS     8
#define RFM69_INT    3
#define RFM69_RST    4
#define RF69_FREQ    915.0

RH_RF69 rf69(RFM69_CS, RFM69_INT);

// === Global Variables ===
char inputBuffer[16];
byte bufferIndex = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("🚀 Feather M0 - SENDER MODE");

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
  Serial.println("✅ RFM69 initialized at 915 MHz");
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r') continue;

    if (c == '\n') {
      inputBuffer[bufferIndex] = '\0';
      handleCommand(inputBuffer);
      bufferIndex = 0;
    } else {
      if (bufferIndex < sizeof(inputBuffer) - 1) {
        inputBuffer[bufferIndex++] = c;
      } else {
        bufferIndex = 0;
        Serial.println("⚠️ Input too long. Buffer cleared.");
      }
    }
  }
}

void handleCommand(const char* cmd) {
  size_t len = strlen(cmd);

  if (strncmp(cmd, "T:", 2) == 0) {
    int val = atoi(cmd + 2);
    if (val >= 0 && val <= 255) {
      uint8_t packet[2] = { 'T', (uint8_t)val };
      rf69.send(packet, 2);
      rf69.waitPacketSent();
      Serial.print("📤 Throttle sent: "); Serial.println(val);
    } else {
      Serial.println("⚠️ Invalid throttle (0-255)");
    }
    return;
  }

  if (strcmp(cmd, "FL") == 0 || strcmp(cmd, "LF") == 0)
    sendDiagonal('F', 'L', "Forward-Left");
  else if (strcmp(cmd, "FR") == 0 || strcmp(cmd, "RF") == 0)
    sendDiagonal('F', 'R', "Forward-Right");
  else if (strcmp(cmd, "BL") == 0 || strcmp(cmd, "LB") == 0)
    sendDiagonal('B', 'L', "Backward-Left");
  else if (strcmp(cmd, "BR") == 0 || strcmp(cmd, "RB") == 0)
    sendDiagonal('B', 'R', "Backward-Right");
  else if (len == 1 && strchr("FBLRS", cmd[0])) {
    char move = cmd[0];
    rf69.send((uint8_t*)&move, 1);
    rf69.waitPacketSent();
    Serial.print("📤 Movement sent: "); Serial.println(move);
  } else {
    Serial.print("⚠️ Unknown command: ");
    Serial.println(cmd);
  }
}

void sendDiagonal(char a, char b, const char* label) {
  uint8_t packet[2] = { a, b };
  rf69.send(packet, 2);
  rf69.waitPacketSent();
  Serial.print("📤 Diagonal sent: "); Serial.println(label);
}
