/*
Programmable, alternative remote control unit for infrared controlled devices
Adafruit Touch Sensor https://www.adafruit.com/product/1982
Remote Library and Code https://www.arduinolibraries.info/libraries/i-rremote
 */

#include "Wire.h" //communication with I2C devices
#include "re" //library touch sensor
#include "IRremote.h" //library ir signals

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

const int RECV_PIN = 2; //pin for ir receiving
IRrecv irrecv(RECV_PIN);
decode_results results;

IRsend irsend;

Adafruit_MPR121 cap = Adafruit_MPR121();

int currtouched = 0; 
int wait = 100; //change value for delay after touches

void setup() {
  Serial.begin(9600); //serial monitor
 
  irrecv.enableIRIn(); //start the receiver
  irrecv.blink13(true);
  
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test:"); 
 
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
    Serial.println("MPR121 found!");
  }

void loop() {

//code for receiving
  if (irrecv.decode(&results)) {
      if (results.decode_type == NEC) { //type of ir signal code
        Serial.print("NEC ");
     } else if (results.decode_type == SONY) {
        Serial.print("SONY ");
     } else if (results.decode_type == RC5) {
        Serial.print("RC5 ");
     } else if (results.decode_type == RC6) {
        Serial.print("RC6 ");
     } else if (results.decode_type == UNKNOWN) {
        Serial.print("UNKNOWN ");
    }
    Serial.print("  ");
    Serial.print(results.value, HEX);
    Serial.print("  ");
    Serial.println(results.bits);
    irrecv.resume(); //receive the next value
  }
  
//code for sending 
  currtouched = cap.touched(); //get the currently touched pads
    
    if (currtouched & _BV(0)) { //button 0 on touch sensor
      Serial.println("0 touched"); //serial monitor output test
      irsend.sendNEC(0x4BB6A05F, 32); //send ir signal, mute
      delay(wait); //wait for defined value
    }
    
    if (currtouched & _BV(1)) {
      Serial.println("1 touched");
      irsend.sendNEC(0x4BB640BF, 32); //vol up (expamples from my TV)
      delay(wait);
    }
    
    if (currtouched & _BV(2)) {
      Serial.println("2 touched");
      irsend.sendNEC(0x4BB6C03F, 32); //vol down
      delay(wait);
    }

    if (currtouched & _BV(3)) {
      Serial.println("3 touched");
      irsend.sendNEC(0x4BB600FF, 32); //prog up
      delay(wait);
    }

     if (currtouched & _BV(4)) {
      Serial.println("4 touched");
      irsend.sendNEC(0x4BB6807F, 32); //prog down
      delay(wait);
    }

    if (currtouched & _BV(5)) {
      Serial.println("5 touched");
      irsend.sendNEC(0x4B36D32C, 32); //on-off
      delay(wait);
    }
  }
