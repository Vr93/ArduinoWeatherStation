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
#define sleepTimeMinutes 10
#define sleepTimeSeconds 0

#define temperature true
#define humidity true
#define pressure true

const String deviceId = "A01";

//===========================================================================
//============================= Variables ===================================
//===========================================================================

int errorCode;
struct WeatherStation {
  String id;
  float temp;
  float hum;
  float pres;
};

Adafruit_BME280 bme; // I2C
#define ledPinRed 2
#define ledPinGreen 3
#define powerRadio 7
#define powerBmeSensor 8


//===========================================================================
//============================== SETUP ======================================
//===========================================================================
void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(ledPinRed,OUTPUT);
  pinMode(ledPinGreen,OUTPUT);
  pinMode(powerRadio,OUTPUT);
  pinMode(powerBmeSensor,OUTPUT);

}

//===========================================================================
//============================= Start Code  =================================
//===========================================================================
void loop() {
  doSleep();  // After finished loop, put all system to sleep.
  turnOnLed();
  digitalWrite(powerBmeSensor,HIGH);
  digitalWrite(powerRadio,HIGH);
  delay(100);
  sendSensorData();
  delay(1000);
  turnOffLed();
  digitalWrite(powerBmeSensor,LOW);
  digitalWrite(powerRadio,LOW);

}
//===========================================================================
//============================= End Code ====================================
//===========================================================================


//===========================================================================
//============================= LED CONTROL =================================
//===========================================================================
void turnOnLed(){
  if(errorCode > 0){  // Turn on the RED LED if there is an error.
    digitalWrite(ledPinRed,HIGH);
  }
  else{               // If no error, turn on the GREEN LED.
    digitalWrite(ledPinGreen,HIGH);
  }
}

void turnOffLed(){
  digitalWrite(ledPinGreen,LOW);
  digitalWrite(ledPinRed,LOW);
}

//===========================================================================
//=============================== SLEEP =====================================
//===========================================================================
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


//===========================================================================
//============================= SEND DATA ===================================
//===========================================================================

void sendSensorData(){
 int bmpError = bme.begin(); // error 0 = no com. with bme280.
 if(bmpError == 0){ // No contact with BMP 280 sensor, set errorCode bit no.0 HIGH.
  bitSet(errorCode,0);  // Activate bit no.0
  
 }
 else{  // Contact with BMP 280 Sensor, set errorCode bit no.1 LOW
    bitClear(errorCode,0);  // Deactivate bit no.0
 }
  Serial.print("id:" + deviceId + ",e:" + String(errorCode));
  delay(100);
  
  if( temperature && (bmpError > 0) ){
  Serial.print("id:" + deviceId + ",t:" + String(bme.readTemperature()));
  delay(100);
  }
  
  if( humidity && (bmpError > 0) ){
    Serial.print("id:" + deviceId + ",h:" + String(bme.readHumidity()));
  delay(100);
  }
  
  if( pressure && (bmpError > 0) ){
    float bme280_pressure = bme.readPressure()/100.0F;
    Serial.print("id:" + deviceId + ",p:" + String(bme280_pressure));
   delay(100);
  }
}

