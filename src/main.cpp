#include <Arduino.h>
#include "Wire.h"
#include "APDS9930.h"

#define pinSensor 7
#define switchPinToIn pinMode(pinSensor, INPUT); 
#define switchPinToOut pinMode(pinSensor, OUTPUT); 

#define APDS9930_INT    2  // Needs to be an interrupt pin
#define LED_PIN         13 // LED for showing interrupt

#define LIGHT_INT_HIGH  1000 // High light level for interrupt
#define LIGHT_INT_LOW   10   // Low light level for interrupt

typedef struct {  //для практики и удобства
  int8_t temperature;
  uint8_t humidity; 
  float ambient;
} dataSensor;

APDS9930 apds = APDS9930();
float ambient_light = 0;
uint16_t ch0 = 0;
uint16_t ch1 = 1;
volatile bool isr_flag = false;
uint16_t threshold = 0;


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

  if (!apds.readAmbientLightLux(data->ambient)) {
    Serial.println(F("Error reading light values"));
  } 
}

void printSensorsData(uint8_t pin){
  dataSensor data;
  readSensors(&data, pin);
  Serial.print("Data sensor: Temperature = ");
  Serial.print(data.temperature);
  Serial.print("C, Humidity = ");
  Serial.print(data.humidity);
  Serial.print("%, Ambient = ");
  Serial.print(data.ambient);
  Serial.println(".");
}

void setup() {
  Serial.begin(115200);
  
  if ( apds.init() ) {
  Serial.println(F("APDS-9930 initialization complete"));
  } 
  else {
    Serial.println(F("Something went wrong during APDS-9930 init!"));
  }

  if ( apds.enableLightSensor(false) ) {
    Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }

  switchPinToOut;
  digitalWrite(pinSensor,HIGH);
  delay(1500);

  pinMode(LED_PIN, OUTPUT);
  pinMode(APDS9930_INT, INPUT);
}

void loop() {
  printSensorsData(pinSensor);
  delay(2000);
}