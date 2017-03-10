//#include <nRF24L01.h>
//#include <RF24.h>
//#include <RF24_config.h>

#define CE_PIN 3
#define CSN_PIN 3
#define SENSOR1 A2
#define CHANNEL 106
#define TIMEOUT 7

uint8_t sensorOneVal = 86;
uint16_t analogReadVal;
//
//RF24 radio(CE_PIN, CSN_PIN);
//uint8_t addresses[][6] = {"1Node", "2Node"};

void setup() {
  //  radio.begin();

    pinMode(SENSOR1, INPUT);
    pinMode(3, OUTPUT);
  //  pinMode(CE_PIN, OUTPUT);
  //  pinMode(CSN_PIN, OUTPUT);

  //  radio.setChannel(CHANNEL);
  //  radio.setPALevel(RF24_PA_LOW);
  //  radio.openWritingPipe(addresses[0]);
  //  radio.openReadingPipe(1, addresses[1]);
}

void loop() {
  //  radio.stopListening();
  analogReadVal = analogRead(SENSOR1);
  delayMicroseconds(20);
  sensorOneVal = map(analogReadVal, 0, 1023, 0, 255);
  if (sensorOneVal >= 128) {
    digitalWrite(3, LOW);
    delay(2);
  } else {
    digitalWrite(3, HIGH);
    delay(2);
  }
  //sensorOneVal = map(analogReadVal, 0, 1023, 0, 255);
  //  sensorOneVal++;
  //  radio.write(&sensorOneVal, sizeof(uint8_t));
  //  delay(3);
  //  radio.startListening();
  //  digitalWrite(CE_PIN, HIGH);
  //powerSave(TIMEOUT);
}
//void powerSave(uint32_t powerOffPeriod) {
//  radio.powerDown();
//  delay(powerOffPeriod * 1000);
//  radio.powerUp();
//  delay(6);
//}
