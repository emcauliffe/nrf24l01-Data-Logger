#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Wire.h>

#define address 72
#define CHANNEL 106

int8_t temp;

RF24 radio(7, 8);
byte addresses[][6] = {"1Node", "2Node"};

void setup() {
  radio.begin();
  Serial.begin(9600);
  Wire.begin();

  ACSR = B10000000;
  ADCSRA = ADCSRA & B01111111;
  DIDR0 = DIDR0 | B00111111;

  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(CHANNEL);

  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
}

void loop() {
  temp = readTemp();
  radio.write(&temp, sizeof(int8_t));
  Serial.println(temp);
  powerSave(7);
}

void powerSave(uint32_t powerOffPeriod) {
  radio.powerDown();
  delay(powerOffPeriod * 1000);
  radio.powerUp();
  delay(6);
}
int8_t readTemp() {
  Wire.beginTransmission(address);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(address, 1);
  while (!Wire.available());
  return Wire.read();
}
