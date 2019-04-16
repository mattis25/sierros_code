
//#include <SoftwareSerial.h>

#include "HardwareSerial.h"
#include <String.h> 
HardwareSerial Serial2(2);
int sms;
 
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

     sms = 0;
     
  //Being serial communication witj Arduino and SIM800
  Serial2.begin(9600);
  delay(1000);

if(Serial2.available()){
  Serial.write(Serial2.read());
  Serial.println("BG");
  }
   
  Serial.println("Setup Complete!");
}

  
  

 
   void loop(){  // Début du loop
   //if(sms==true){
     //sendsms();       
     //sms = false;
   //}

  while(sms < 5){
    sendsms();
    sms = sms + 1;
    delay(1000);
    }
   
 }
   
  void sendsms(){
    Serial.println("Sending text message...");
    Serial2.print("AT+CMGF=1\r");  // Lance le mode SMS
    delay(100);
    // Entrez votre numéro de téléphone    
    Serial2.print("AT+CMGS=\"+33695011092\"\r");  
    delay(100);
    // Entrez votre message ici    
    Serial2.print("Hello world SIERROS \r");  
    // CTR+Z en langage ASCII, indique la fin du message
    Serial2.print(char(26));        
    delay(100);
    Serial2.println();
    Serial.println("Text send");  // Le message est envoyé.
    }
