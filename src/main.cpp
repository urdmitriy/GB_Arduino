#include <Arduino.h>

#define pinButton 2
#define pinLed  7

int buttonState1 = 0, buttonState2 = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinLed,OUTPUT);
//  pinMode(pinButton,INPUT_PULLUP); // при использовании подтяжки микроконтроллера
  pinMode(pinButton, INPUT); //при использовании внешней подтяжки
  
}

void loop() {
  // put your main code here, to run repeatedly:

  buttonState1 = digitalRead(pinButton);
  delay(300);
  buttonState2 = digitalRead(pinButton);

  if (buttonState1 == buttonState2){
    if (!(buttonState1 == HIGH)){
      digitalWrite(pinLed,HIGH);
      } else{
      digitalWrite(pinLed,LOW);
    }
  }
  

}