#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <Wire.h>
#include <EEPROM.h>

#define CHANNEL 106

#define DATAPIN 4
#define LATCHPIN 3
#define CLKPIN 2

int8_t receivedData;
uint8_t eepromAddress;

RF24 radio(7, 8);
uint8_t wirelessAddresses[][6] = {"1Node", "2Node"};

void setup() {

  // initalize shift register pins
  pinMode(DATAPIN, OUTPUT);
  pinMode(LATCHPIN, OUTPUT);
  pinMode(CLKPIN, OUTPUT);


  // initialize libraries
  Wire.begin();
  Serial.begin(9600);
  radio.begin();

  // set radio settings
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(CHANNEL);

  // select addresses to read and write from
  radio.openWritingPipe(wirelessAddresses[1]);
  radio.openReadingPipe(1, wirelessAddresses[0]);
  radio.startListening();
  
}

void loop() {

  // whenever a tempature is available
  // place into eeprom and display it on bargraph
  
  if (radio.available()) {
    
    radio.read(&receivedData, 1);
    
    Serial.println(receivedData);

    if (eepromAddress == EEPROM.length()) eepromAddress = 0;
    EEPROM.write(eepromAddress, receivedData);
    eepromAddress++;
    
    displayTemp(map(receivedData, -20, 80, 0, 8));
    
  }
}

void displayTemp (uint8_t data) {
  
  uint8_t shiftData = 0;

  for (uint8_t n = 0; n < data; n ++) shiftData |= (1 << n);

  shiftOut(DATAPIN, CLKPIN, LSBFIRST, shiftData);
  digitalWrite(LATCHPIN, HIGH);
  digitalWrite(LATCHPIN, LOW);
  
}
