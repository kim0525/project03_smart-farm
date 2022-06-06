#include <SimpleDHT.h>
#include <Servo.h>

SimpleDHT11 dht11;
Servo myservo;

//led
const int pinLed = 12;


const int pinDHT11 =   2; // pin No.: DHT11 temp & humi sensor connected
const int pinMoist =  A0; // pin No.: DHT11 soil misture sensor connected
const int pinServo =   9; // pin No.: servo motor connected
const int pinFan   =   4; // pin No.: dc fan connected
const int pinSol   =   5; // pin No.: solenoid valve connected

const int pos0     =  75; // position0: vent shutter close
const int pos1     =  87; // position1: vent shutter open medium
const int pos2     = 108; // position2: vent shutter open maximum

byte temp = 0, humi = 0;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
unsigned int ledStatus = 0;

void getTnH(void);

void setup() {
  
  //led
  pinMode(pinLed,OUTPUT);
  digitalWrite(pinLed,LOW);
  
  myservo.attach(pinServo);  myservo.write(pos0);  delay(100);
  pinMode(pinFan, OUTPUT);  pinMode(pinFan, HIGH);
  pinMode(pinSol, OUTPUT);  pinMode(pinSol, HIGH);
  Serial.begin(9600);
  Serial.println("\n*** Smart Greenhouse ***\n");
  Serial.println("--------------------------");
  inputString.reserve(200);
}

void loop()
{
  if(Serial.available() > 0) {
    unsigned char ch = Serial.read();
    if     (ch == '1') digitalWrite(pinLed,HIGH);
    else if(ch == '0') digitalWrite(pinLed,LOW );
    else;
  }

  unsigned int mois = analogRead(pinSol);
  getTnH();
  
  Serial.print("TEMP");  Serial.println(temp%100);  // send 2 digit number
  Serial.print("HUMI");  Serial.println(humi%100);  // send 2 digit number
  Serial.print("MOIS");  Serial.println(mois);
  Serial.println("--------------------------");
  
  if (stringComplete) 
  {
    //// when recive string start "SERVO"
    if( inputString.substring(0,5) == "SERVO" )
    {
      if( inputString.substring(5,6) == "0" ) {       // close window
        myservo.write(pos0);  delay(100);  Serial.println("SERVO0"); // send state
      }
      else if( inputString.substring(5,6) == "1" ) {  // open window to positon1
        myservo.write(pos1);  delay(100);  Serial.println("SERVO1"); // send state
      }
      else if( inputString.substring(5,6) == "2" ) {  // open window to positon1
        myservo.write(pos2);  delay(100);  Serial.println("SERVO2"); // send state
      }
      else;
    }
    //// when recive string start "DCFAN"
    else if( inputString.substring(0,5) == "DCFAN" )
    {
      if( inputString.substring(5,6) == "0" ) {       // FAN off
        digitalWrite(pinFan, LOW );  Serial.println("DCFAN0"); // send state
      }
      else if( inputString.substring(5,6) == "1" ) {  // FAN on
        digitalWrite(pinFan, HIGH);  Serial.println("DCFAN1"); // send state
      }
      else;
    }
    //// when recive string start "VALVE"
    else if( inputString.substring(0,5) == "VALVE" ) {
      if( inputString.substring(5,6) == "0" ) {       // open VALVE
        digitalWrite(pinSol, LOW );  Serial.println("VALVE0"); // send state
      }
      else if( inputString.substring(5,6) == "1" ) {  // close VALVE
        digitalWrite(pinSol, HIGH);  Serial.println("VALVE1"); // send state
      }
      else;
    }
    inputString = "";
    stringComplete = false;
  }
  delay(1500);  // DHT11 sensor needs more than 1(sec) to process
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void getTnH(){
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinDHT11, &temp, &humi, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err); delay(1000);
    return;
  }
}


//const int pinLed = 12;
//
//void setup() {
//  pinMode(pinLed,OUTPUT);
//  digitalWrite(pinLed,LOW);
//  Serial.begin(9600);
//}
//
//void loop() {
//  if(Serial.available() > 0) {
//    unsigned char ch = Serial.read();
//    if     (ch == '1') digitalWrite(pinLed,HIGH);
//    else if(ch == '0') digitalWrite(pinLed,LOW );
//    else;
//  }
//}
