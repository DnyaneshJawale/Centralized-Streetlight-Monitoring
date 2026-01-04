#include <SPI.h>
#include <LoRa.h>

/* ================= CONFIG ================= */

#define NODE_ID "SL-001"

/* pins */
#define PIN_PWM        5
#define PIN_PIR        2
#define PIN_LDR_LAMP   A0
#define PIN_LDR_AMB    A1
#define PIN_VOLT       A2
#define PIN_CURR       A3

#define LORA_SS  10
#define LORA_RST 9
#define LORA_DIO0 8

/* thresholds */
#define AMBIENT_DARK_TH     500
#define LAMP_ON_TH          300
#define OVER_CURRENT_TH     1.2     // amps
#define UNDER_VOLT_TH       10.5    // volts

#define PWM_FULL 255
#define PWM_DIM  150

#define IDLE_DIM_TIME_MS 30000

/* ================= STATE ================= */

unsigned long lastMotionTime = 0;
uint8_t pwmLevel = PWM_FULL;
uint8_t faultCode = 0;

/* ================= UTILS ================= */

float readVoltage() {
  int raw = analogRead(PIN_VOLT);
  float v = (raw * 5.0 / 1023.0) * 5.0; // divider calibrated
  return v;
}

float readCurrent() {
  int raw = analogRead(PIN_CURR);
  float voltage = raw * (5.0 / 1023.0);
  float current = (voltage - 2.5) / 0.185; // ACS712-5A
  return abs(current);
}

/* ================= FAULT LOGIC ================= */

void detectFaults(float v, float i, int lampLdr) {
  faultCode = 0;

  if (pwmLevel > 0 && lampLdr < LAMP_ON_TH)
    faultCode = 1; // lamp off

  else if (i > OVER_CURRENT_TH)
    faultCode = 2;

  else if (v < UNDER_VOLT_TH)
    faultCode = 3;

  else if (v > UNDER_VOLT_TH && i < 0.05)
    faultCode = 4;
}

/* ================= LORA ================= */

void sendPacket(float v, float i) {
  LoRa.beginPacket();
  LoRa.print("{\"id\":\"");
  LoRa.print(NODE_ID);
  LoRa.print("\",\"pwm\":");
  LoRa.print(pwmLevel);
  LoRa.print(",\"v\":");
  LoRa.print(v, 2);
  LoRa.print(",\"i\":");
  LoRa.print(i, 2);
  LoRa.print(",\"fault\":");
  LoRa.print(faultCode);
  LoRa.print("}");
  LoRa.endPacket();
}

/* ================= SETUP ================= */

void setup() {
  pinMode(PIN_PWM, OUTPUT);
  pinMode(PIN_PIR, INPUT);

  analogWrite(PIN_PWM, PWM_FULL);

  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    while (1);
  }

  lastMotionTime = millis();
}

/* ================= LOOP ================= */

void loop() {

  int ambient = analogRead(PIN_LDR_AMB);
  int lampLdr = analogRead(PIN_LDR_LAMP);

  bool motion = digitalRead(PIN_PIR);

  if (ambient < AMBIENT_DARK_TH) {

    if (motion) {
      pwmLevel = PWM_FULL;
      lastMotionTime = millis();
    } 
    else if (millis() - lastMotionTime > IDLE_DIM_TIME_MS) {
      pwmLevel = PWM_DIM;
    }

    analogWrite(PIN_PWM, pwmLevel);
  } 
  else {
    pwmLevel = 0;
    analogWrite(PIN_PWM, 0);
  }

  float voltage = readVoltage();
  float current = readCurrent();

  detectFaults(voltage, current, lampLdr);
  sendPacket(voltage, current);

  delay(2000);
}
