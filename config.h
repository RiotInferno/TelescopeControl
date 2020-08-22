#include <SPI.h>
#include <U8x8lib.h>
#include <SoftwareSerial.h>
#include "GPS.h"
#include "Heading.h"

U8X8_SSD1306_128X64_NONAME_HW_I2C display(/* reset=*/U8X8_PIN_NONE);
GPS gps;
Heading mpu9250;



#define AHRS true         // set to false for basic data read
#define SerialDebug false // set to true to get Serial output for debugging

#pragma region GLOBALS !

// Pin definitions
int intPin = 12; // These can be changed, 2 and 3 are the Arduinos ext int pins
int myLed = 13;  // Set up pin 13 led for toggling


char input[20];
char txAR[10];
char txDEC[11];
static const int TXPin = 4, RXPin = 3;
#pragma endregion