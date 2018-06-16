//===========================================================================
//============================= Libraries ===================================
//===========================================================================

#include "LowPower.h"
#include <Adafruit_BME280.h>
#include <Wire.h>

//===========================================================================
//============================= Settings ====================================
//===========================================================================

#define sleepTimeHours 0
#define sleepTimeMinutes 0
#define sleepTimeSeconds 10

#define temperature true
#define humidity true
#define pressure true

//===========================================================================
//============================= Variables ===================================
//===========================================================================

struct WeatherStation {
  String id;
  float temp;
  float hum;
  float pres;
};

Adafruit_BME280 bme; // I2C
#define ledPin 3
#define powerRadio 7
#define powerBmeSensor 8


void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(ledPin,OUTPUT);
  pinMode(powerRadio,OUTPUT);
  pinMode(powerBmeSensor,OUTPUT);

}

void loop() {
  doSleep();  // After finished loop, put all system to sleep.
//===========================================================================
//============================= Start Code  =================================
//===========================================================================
  digitalWrite(ledPin,HIGH);
  digitalWrite(powerBmeSensor,HIGH);
  digitalWrite(powerRadio,HIGH);
  delay(100);
  sendSensorData();
  delay(1000);
  digitalWrite(ledPin,LOW);
  digitalWrite(powerBmeSensor,LOW);
  digitalWrite(powerRadio,LOW);

//===========================================================================
//============================= End Code ====================================
//===========================================================================
}


void doSleep() {
  #if sleepTimeSeconds > 60
    #error sleepTimeSeconds is greater than 60!
  #endif
  #if sleepTimeSeconds < 0
    #error sleepTimeSeconds is negative, must be positive!
  #endif
  #if sleepTimeMinutes > 60
    #error sleepTimeMinutes is greater than 60!
  #endif
  #if sleepTimeMinutes < 0
    #error sleepTimeMinutes is negative, must be positive!
  #endif
  #if sleepTimeHours < 0
    #error sleepTimeHours is negative, must be positive!
  #endif 

  long secondsToSleep = (sleepTimeSeconds) + (sleepTimeMinutes*60) + (sleepTimeHours*3600);
  for (long s = 0; s < secondsToSleep; s++) {
        LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
      } 
}

void sendSensorData(){
 int error = bme.begin(); // error 0 = no com. with bme280.
  Serial.print("id:A01,e:" + String(error));
  delay(100);
  if( temperature && (error > 0) ){
  Serial.print("id:A01,t:" + String(bme.readTemperature()));
  delay(100);
  }
  if( humidity && (error > 0) ){
    Serial.print("id:A01,h:" + String(bme.readHumidity()));
  delay(100);
  }
  if( pressure && (error > 0) ){
    float bme280_pressure = bme.readPressure()/100.0F;
    Serial.print("id:A01,p:" + String(bme280_pressure));
   delay(100);
  }
  

}

