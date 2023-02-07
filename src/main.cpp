#include <Arduino.h>
#include "SPI.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

#define pinSensor 7
#define switchPinToIn pinMode(pinSensor, INPUT); 
#define switchPinToOut pinMode(pinSensor, OUTPUT); 

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

const uint16_t  Display_Color_Black        = 0x0000;
const uint16_t  Display_Color_Blue         = 0x001F;
const uint16_t  Display_Color_Red          = 0xF800;
const uint16_t  Display_Color_Green        = 0x07E0;
const uint16_t  Display_Color_Cyan         = 0x07FF;
const uint16_t  Display_Color_Magenta      = 0xF81F;
const uint16_t  Display_Color_Yellow       = 0xFFE0;
const uint16_t  Display_Color_White        = 0xFFFF;

volatile uint8_t flagButtonPushed = 0;


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

typedef struct {  //для практики и удобства
  int8_t temperature;
  uint8_t humidity; 
} dataSensor;

dataSensor data;

void interruptHandler(void){
  // flagButtonPushed = 1; //если нужно обработать прерывание по флагу
  Serial.print("Data sensor: Temperature = ");
  Serial.print(data.temperature);
  Serial.print("C, Humidity = ");
  Serial.print(data.humidity);
  Serial.println("%.");
}

uint8_t waitLine(uint8_t waitValue, uint16_t maxTime, uint8_t pin){
  unsigned long timeStart = micros();
  unsigned long length;
  while (digitalRead(pin)==waitValue && (length = micros()-timeStart)<maxTime);
  if (length > maxTime){
    return 1; //ошибка
  }
  return 0;
}

int8_t readWordFromSensor(uint8_t pin){
  int16_t data = 0;
  uint8_t error = 0;
  for (size_t i = 0; i < 16; i++)
  {
    error += waitLine(0,100,pinSensor); //ждем окончания "синхроимпульса"
    unsigned long startTime = micros(); //измеряем время имульса данных 
    error += waitLine(1,100,pinSensor); //wait 1 or 0
    
    if (micros() - startTime > 40) { //Если импульс данных длиннее 40 (26-28) мкс, то бит = 1
      data |= (1 << (15-i));
    }
  }

  if (error == 0){
    return data>>8; //если ошибки не было, выдаем старший байт
  }else{
    Serial.println("Error sensor");
    return 0;
  }
}

void readSensors(dataSensor * data, uint8_t pin){
  switchPinToOut;
  digitalWrite(pin, LOW);  //
  delay(20);               // стартовый импульс
  digitalWrite(pin, HIGH); //
  switchPinToIn;
  uint8_t error = 0;
  error += waitLine(1,100,pinSensor); // ждем начало импульса ответа
  error += waitLine(0,100,pinSensor); // ждем окончания импульса ответа
  error += waitLine(1,100,pinSensor); //  ждем начала передачи данных

  if (error > 0){
    data->humidity =  readWordFromSensor(pin);
    data->temperature = readWordFromSensor(pin);
  } else {
    data->humidity =  0;
    data->temperature = 0;
  }

  data->humidity =  readWordFromSensor(pin);
  data->temperature = readWordFromSensor(pin);
}

void printSensorsData(uint8_t pin){
  static dataSensor oldData;
  readSensors(&data, pin);

  if (data.temperature != oldData.temperature || data.humidity != oldData.humidity) {
    //стираем старые данные
    tft.setTextColor(0x0000);
    tft.setCursor(50,0);
    tft.print(oldData.temperature);
    tft.setCursor(50,40);
    tft.print(oldData.humidity);

    //пишем новые данные
    tft.setTextColor(data.humidity > 30 ? Display_Color_Red : Display_Color_Yellow);
    tft.setCursor(50,0);
    tft.print(data.temperature);
    tft.setCursor(50,40);
    tft.print(data.humidity);
  }
  
  if (flagButtonPushed){ //если нужно обработать прерывание по флагу
    //пишем в порт
    // Serial.print("Data sensor: Temperature = ");
    // Serial.print(data.temperature);
    // Serial.print("C, Humidity = ");
    // Serial.print(data.humidity);
    // Serial.println("%.");
    // flagButtonPushed = 0;
  }
  
  oldData.humidity = data.humidity;
  oldData.temperature = data.temperature;
}

void setup() {
  Serial.begin(115200);
  switchPinToOut;
  digitalWrite(pinSensor,HIGH);
  
  delay(1500);
  
  tft.initR(INITR_144GREENTAB);
  tft.setFont();
  tft.fillScreen(Display_Color_Black);
  tft.setTextColor(Display_Color_White);
  tft.setTextSize(4);

  tft.setCursor(0,0);
  tft.print("t=");
  tft.setCursor(100,0);
  tft.print("C");
  tft.setCursor(0,40);
  tft.print("h=");
  tft.setCursor(100,40);
  tft.print("%");

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, interruptHandler, FALLING);

}


void loop() {
  printSensorsData(pinSensor);
  tft.setTextSize(2);
  uint8_t y = 117;
  uint8_t d = 10;
  for (size_t i = 10; i < 120; i++)
  {
    tft.drawCircle(i, y, d, Display_Color_Cyan);
    delay(10);
    tft.drawCircle(i, y, d, Display_Color_Black);
  }

  for (size_t i = 120; i >= 10; i--)
  {
    tft.drawCircle(i, y, d, Display_Color_Cyan);
    delay(10);
    tft.drawCircle(i, y, d, Display_Color_Black);
  }
}