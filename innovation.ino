#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <SD.h>    
#include <TMRpcm.h>                   
//#define SD_ChipSelectPin 53  
#define SD_ChipSelectPin 4  
         


TMRpcm tmrpcm; 

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10
#define scrollSpeed  70   // scroll speed
#define scrollPause  0    // ms of pause after finished displaying message

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// sets scrolling direction if slider in middle at start
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;  // how to align the text

#define  BUF_SIZE  75  // Maximum of 75 characters
#define trigPin  A0
#define echoPin  A1

char testStr[10] = { "" };
String heightStr = "";
long duration;
int distance;
int height;
int x=0;
int l=28;
int i=0;
void xx(int high);
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  P.begin();
  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
 
  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");
    return;   // don't do anything more if not
  } Serial.println("SD");
}

void loop() {
  
  if(l==28){
  measure();}
   Serial.println(l);
  if (distance > 60) {
    x=0;
    P.displayText("TINGGI", PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
    Serial.print("tinggi");
  } else {
    x++;
    if(x>120){x=0;l=28;}
    if(x>80){         
    l=15;
    height = 180 - distance;
    Serial.print(height);
    //duration = pulseIn(echoPin, LOW);
      if(x>90&&x<100){
        xx(height);
        Serial.println(i);
     
        }   

      }
     
    height = 180 - distance;
    heightStr = String(height, DEC) + "cm";
    heightStr.toCharArray(testStr,75);
    P.displayText(testStr, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  }
  P.displayAnimate();
//  delay(100);
}
void xx(int high){
    Serial.println(x);
    
      if(high>100){
      tmrpcm.play("2.wav");        
      Serial.println("pp");        
      }
  }
  
void measure(){
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(12);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.017;
 
  
}
