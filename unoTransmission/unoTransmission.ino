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

  //turn off the ADC to conserve power
  ACSR = B10000000;
  ADCSRA = ADCSRA & B01111111;
  DIDR0 = DIDR0 | B00111111;

  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(CHANNEL);

  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
}
void loop() {
  temp = readTemp();/
  radio.write(&temp, sizeof(int8_t));
  powerSave(7);//low power mode for 7 seconds
}
void powerSave(uint32_t powerOffPeriod) {
  radio.powerDown();//places wireless chip into ultra low power mode
  delay(powerOffPeriod * 1000);
  radio.powerUp();
  delay(6);//wait for startup
}
int8_t readTemp() {
  //prepare to read from sensor
  Wire.beginTransmission(address);
  Wire.write(0);
  Wire.endTransmission();
  //read data from sensor
  Wire.requestFrom(address, 1);
  while (!Wire.available());
  return Wire.read();
}
