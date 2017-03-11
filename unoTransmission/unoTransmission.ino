#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <Wire.h>

#define CHANNEL 106
#define TEMPSENSORADDRESS 72

int8_t currentTemp;

RF24 radio(7, 8);
byte addresses[][6] = {"1Node", "2Node"};

void setup() {

  // initialize libraries
  radio.begin();
  Serial.begin(9600);
  Wire.begin();

  //turn off the ADC to conserve power
  ACSR = B10000000;
  ADCSRA = ADCSRA & B01111111;
  DIDR0 = DIDR0 | B00111111;

  // set radio settings
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(CHANNEL);

  // select addresses to read and write from
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  
}

void loop() {
  
  currentTemp = readTemp();
  radio.write(&currentTemp, sizeof(int8_t));
  powerSave(6); // enter low power mode for 7 seconds
  
}

void powerSave(uint32_t powerOffPeriod) {
  
  radio.powerDown();
  delay(powerOffPeriod * 1000);
  
  radio.powerUp();
  delay(6); // delay as radio boots
  
}

int8_t readTemp() {
  
  // prepare wire library to read from temperature sensor
  Wire.beginTransmission(TEMPSENSORADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  
  // read temperature from sensor
  Wire.requestFrom(TEMPSENSORADDRESS, 1);
  while (!Wire.available());
  return Wire.read();
  
}
