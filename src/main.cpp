#include <Arduino.h>

#define pinButton 2
#define pinLed  7

int buttonState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinLed,OUTPUT);
//  pinMode(pinButton,INPUT_PULLUP); // при использовании подтяжки микроконтроллера
  pinMode(pinButton, INPUT); //при использовании внешней подтяжки
  
}

void loop() {
  // put your main code here, to run repeatedly:

  buttonState = digitalRead(pinButton);

  if (!(buttonState == HIGH)){
    digitalWrite(pinLed,HIGH);
    delay(1); //использование задержки для ожидания установившегося контакта
  } else{
    digitalWrite(pinLed,LOW);
  }

}