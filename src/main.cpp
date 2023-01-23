#include <Arduino.h>

#define PinLed1 12
#define PinLed2 10
#define PinLed3 8
#define PinResistor A0
#define PinPhotoResistor A5


int16_t valuePhotoSensor = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PinLed1, OUTPUT);
  pinMode(PinLed2, OUTPUT);
  pinMode(PinLed3, OUTPUT);
  Serial.begin(115200);
}

void switchLed(uint16_t value){
if (value >= 0 && value < 340){
    digitalWrite(PinLed1, HIGH);
    digitalWrite(PinLed2, LOW);
    digitalWrite(PinLed3, LOW);
  }
  else if (value>=340 && value < 680) {
    digitalWrite(PinLed1, LOW);
    digitalWrite(PinLed2, HIGH);
    digitalWrite(PinLed3, LOW);
  }
  else {
    digitalWrite(PinLed1, LOW);
    digitalWrite(PinLed2, LOW);
    digitalWrite(PinLed3, HIGH);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
 
  switchLed(analogRead(PinResistor));

  valuePhotoSensor += (analogRead(PinPhotoResistor) - valuePhotoSensor) * 0.1;
  
  Serial.println(valuePhotoSensor*10/1023);

}