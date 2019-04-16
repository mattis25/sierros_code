//#include <SoftwareSerial.h>

#include "HardwareSerial.h"
HardwareSerial Serial2(2);
 
//SIM800 TX is connected to Arduino D8
//#define SIM800_TX_PIN 23
 
//SIM800 RX is connected to Arduino D7
//#define SIM800_RX_PIN 25
 
//Create software serial object to communicate with SIM800
//SoftwareSerial serialSIM800(SIM800_TX_PIN,SIM800_RX_PIN);
 
void setup() {
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  while(!Serial);
   
  //Being serial communication witj Arduino and SIM800
  Serial2.begin(9600);
  delay(1000);
   
  Serial.println("Setup Complete!");
}
 
void loop() {
  //Read SIM800 output (if available) and print it in Arduino IDE Serial Monitor
  if(Serial2.available()){
   // Serial.println("ok bitch");
    Serial.write(Serial2.read());
  }
  else{
  //  Serial.println("Fail");
    }
  //Read Arduino IDE Serial Monitor inputs (if available) and send them to SIM800
  if(Serial.available()){    
    Serial2.write(Serial.read());
  }

//delay(500);

}
