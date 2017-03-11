#include <EEPROM.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Wire.h>

#define CHANNEL 106

#define DATAPIN 4
#define LATCHPIN 3
#define CLKPIN 2

int8_t receivedData;
uint16_t EEPROMAddress = 0;

RF24 radio(7, 8);
uint8_t wirelessAddresses[][6] = {"1Node", "2Node"};

void setup() {
  //configure pins for shift register
  pinMode(DATAPIN, OUTPUT);
  pinMode(LATCHPIN, OUTPUT);
  pinMode(CLKPIN, OUTPUT);

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
  if (radio.available()) {
    radio.read(&receivedData, 1);
    Serial.println(receivedData);
    EEPROMAddress++;
    displayTemp(map(receivedData, -20, 80, 0, 8));//maps temperatures from -20C to 80C on the bar graph
  }
}
void displayTemp (uint8_t data) {
  uint8_t shiftData = 0;
  for (uint8_t n = 0; n < data; n ++) {
    shiftData |= (1 << n);
  }
  shiftOut(DATAPIN, CLKPIN, LSBFIRST, shiftData);
  digitalWrite(LATCHPIN, HIGH);
  digitalWrite(LATCHPIN, LOW);
}
