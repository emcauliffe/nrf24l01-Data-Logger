#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Wire.h>

#define DISK1 0x50 //address of EEPROM chip
#define CHANNEL 83

uint8_t inputData;
uint16_t address = 0;

RF24 radio(7, 8);
uint8_t wirelessAddresses[][6] = {"1Node", "2Node"};

void setup() {
  Wire.begin();
  Serial.begin(9600);
  radio.begin();

  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(CHANNEL);

  radio.openWritingPipe(wirelessAddresses[1]);
  radio.openReadingPipe(1, wirelessAddresses[0]);
  radio.startListening();
}

void loop() {
  inputData = readWireless();
  writeEEPROM(DISK1, address, inputData);
  address++;
}
uint8_t readWireless() {
  uint8_t received;
  while (radio.available()) {
    radio.read(&received, sizeof(uint8_t));
  }
  return received;
}
void writeEEPROM(int16_t deviceaddress, uint16_t eeaddress, uint8_t data ) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));// MSB
  Wire.write((int)(eeaddress & 0xFF));// LSB
  Wire.write(data);
  Wire.endTransmission();
  delay(5);
}
