#include <Arduino.h>

#define pinSER 10
#define pinRCLK 8
#define pinSRCLK 9

void sendTo595(uint16_t value){
  for (size_t i = 0; i < 9; i++) 
  {
    if (value & 0x01 << (8-i)){   // раскладываем побитно value
      digitalWrite(pinSER, HIGH);  // если 1, то SER в высокий уровень
    }
    else {
      digitalWrite(pinSER, LOW);  // если 0, то SER в низкий уровень
    }
    
    digitalWrite(pinSRCLK,HIGH);  //помещаем бит в сдвиговый регистр
    delayMicroseconds(1);
    digitalWrite(pinSRCLK,LOW);
    delayMicroseconds(1);
  }

  digitalWrite(pinRCLK,HIGH);    //помещаем данные из сдвигового регистра в регистр хранения
  delayMicroseconds(1);
  digitalWrite(pinRCLK,LOW);
  delayMicroseconds(1);
}

void printDigit(uint8_t value){
  uint8_t arrayChar[10] = {0b00111111, //0
                          0b00000110,  //1
                          0b01011011,  //2
                          0b01001111,  //3
                          0b01100110,  //4
                          0b01101101,  //5
                          0b01111101,  //6
                          0b00000111,  //7
                          0b01111111,  //8
                          0b01101111}; //9

  sendTo595(arrayChar[value]);
}

void setup() {
  pinMode(pinSER, OUTPUT);
  pinMode(pinRCLK, OUTPUT);
  pinMode(pinSRCLK, OUTPUT);
}

void loop() {
  for (size_t i = 0; i < 10; i++)
  {
    printDigit(i);
    delay(500);
  }
}