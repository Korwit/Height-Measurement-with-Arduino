/*
Korwit Kolang M.5/5 
Project: Measure your height with ultrasonic sensor and display it on LED matrix
*/
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN 13
#define DATA_PIN 11
#define CS_PIN 10
#define scrollSpeed 70
#define scrollPause 0


MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;

#define BUF_SIZE 75
#define trigPin A0
#define echoPin A1

int buzzer = 2;
char testStr[10] = { "" };
String heightStr = "";
long duration;
int distance, height, i = 0;


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);  //I use buzzer for notice that measure process is finish, So when it is finish the buzzer will make sound beep.
  digitalWrite(buzzer, HIGH);
  Serial.begin(9600);
  P.begin();
}

void loop() {

  measure();             // Measure distance from ultrasonic sensor
  if (distance > 100) {  // If no person detected within 100 cm, reset counter and display READY
    i = 0;
    P.displayText("READY", PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
  } else {
    // Person detected within 100 cm
    i++;

    // If person has been detected for 5 cycles, beep twice quickly
    if (i == 5) {
      digitalWrite(buzzer, LOW);
      delay(50);
      digitalWrite(buzzer, HIGH);
      delay(120);
      digitalWrite(buzzer, LOW);
      delay(50);
      digitalWrite(buzzer, HIGH);
    }
    height = 209 - distance;                                          // Calculate height (assuming ceiling height is 209 cm)
    heightStr = String(height, DEC) + "cm";                           // Convert height to string with "cm" unit
    heightStr.toCharArray(testStr, 75);                               // Convert String to char array for MD_Parola
    P.displayText(testStr, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);  // Display the height on LED matrix
    if (i > 50) {                                                     // If person stands still for more than 50 cycles (~5 sec), show final height and beep
      heightStr = String(height, DEC) + "cm";
      Serial.println(height);
      heightStr.toCharArray(testStr, 75);
      P.displayText(testStr, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);


      digitalWrite(buzzer, LOW);  // Long beep for measurement completion
      delay(1000);
      digitalWrite(buzzer, HIGH);
      delay(6000);

      // Reset counter after one-time display
      if (i == 51)
        i = 0;
    }
  }

  // Update LED display animation
  P.displayAnimate();
}

void measure() {  // Function for measure your height in cm.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.017;
}
