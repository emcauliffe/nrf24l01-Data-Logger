#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Wire.h>

#define CHANNEL 83

#define DATAPIN 4
#define LATCHPIN 3
#define CLKPIN 2

uint8_t receivedData;
uint8_t displayData;
uint16_t address = 0;

RF24 radio(7, 8);
uint8_t wirelessAddresses[][6] = {"1Node", "2Node"};

void setup() {
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
  receivedData = readWireless();
  Serial.println(receivedData);
  displayData = map(receivedData, 0, 255, 0, 8);
  displayTemp(displayData);
}

uint8_t readWireless() {
  uint8_t received;
  while (radio.available()) {
    radio.read(&received, sizeof(uint8_t));
  }
  return received;
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

