#include <Arduino.h>

#define pinLedRed 10
#define pinLedGreen 6
#define pinLedBlue 5
#define pinResistor A0
#define delayColor 3000

uint8_t ledArray[3] = {pinLedRed, pinLedGreen, pinLedBlue};
unsigned long timeStamp = millis();
uint8_t numberActiveLed = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinLedRed, OUTPUT);
  pinMode(pinLedGreen, OUTPUT);
  pinMode(pinLedBlue, OUTPUT);
}

uint16_t readData(void){
  static int32_t valuePhotoSensor = 0;
  valuePhotoSensor += (analogRead(pinResistor) - valuePhotoSensor) * 0.1;
  return valuePhotoSensor*255/1023;
}

void lightLed(uint8_t pinLed, uint8_t valueLight){

  if (pinLed == pinLedRed) {
    analogWrite(pinLedRed, valueLight); 
    analogWrite(pinLedBlue, 0); 
    analogWrite(pinLedGreen, 0);
  } else if (pinLed == pinLedGreen) {
    analogWrite(pinLedGreen, valueLight); 
    analogWrite(pinLedBlue, 0); 
    analogWrite(pinLedRed, 0);
  } else{
    analogWrite(pinLedBlue, valueLight); 
    analogWrite(pinLedGreen, 0); 
    analogWrite(pinLedRed, 0);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (millis() - timeStamp > delayColor){
    if (numberActiveLed<2){
      numberActiveLed++;
    }
    else{
      numberActiveLed = 0;
    }
    timeStamp = millis();
  }
  
  lightLed(ledArray[numberActiveLed], readData());
}