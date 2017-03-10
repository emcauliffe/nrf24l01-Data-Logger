// Made by Ethan & Tim

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#define CE_PIN 3
#define CSN_PIN 3//slave select pin
#define SENSOR1 4
#define CHANNEL 83
#define TIMEOUT 7

uint8_t sensorOneVal;
uint32_t watchdogCounter;

RF24 radio(CE_PIN, CSN_PIN);
uint8_t addresses[][6] = {"1Node", "2Node"};

void setup() {
  radio.begin();
  radio.setChannel(CHANNEL);
  radio.openWritingPipe(addresses[0]);
}

void loop() {
  sensorOneVal = map(analogRead(SENSOR1), 0, 1023, 0, 255);
  radio.write(&sensorOneVal, sizeof(uint8_t));
  powerSave(TIMEOUT);
}
void powerSave(uint32_t powerOffPeriod) {
  radio.powerDown();
  delay(powerOffPeriod*1000);
  //radio.powerUp();
  //delay(6);
}

