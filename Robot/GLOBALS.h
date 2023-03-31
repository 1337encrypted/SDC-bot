#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "tb6612fng.h"
#include "config.h"
#include <Servo.h>

//Servo Pin
constexpr uint8_t sliderPin = 3;

//Left Motor pins
constexpr uint8_t IN1 = A1;
constexpr uint8_t IN2 = A0;
constexpr uint8_t PWM1 = 5;
constexpr uint8_t STDBY1 = 2;

//Right Motor pins
constexpr uint8_t IN3 = A2;
constexpr uint8_t IN4 = A3;
constexpr uint8_t PWM2 = 6;
constexpr uint8_t STDBY2 = -1;

//Radio communication pins
constexpr uint8_t CE = 10;
constexpr uint8_t CSN = 9;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct dataPackage 
{
  int j1PotX;
  int j1PotY;
  byte j1Button;
  int j2PotX;
  int j2PotY;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte button1;
  byte button2;
  byte button3;
  byte button4;
};

dataPackage data; //Create a variable with the above structure

//Servo object 
Servo slider;

//Motor object
tb6612fng leftMotor(IN1,IN2,PWM1,STDBY1,1);
tb6612fng rightMotor(IN3,IN4,PWM2,STDBY2,2);

//Radio communication objects
RF24 radio(10,9);   // nRF24L01 (CE, CSN)
const uint64_t pipe = 0x1; // This is the transmit pipeline

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

//Miscellaneous 
int minPosDeadBand = 10;
int minNegDeadBand = -10;

enum class servoStates : uint8_t
{
  SERVOFRONT,
  SERVOBACK,
  IDLE
};
servoStates servoStatus = servoStates::IDLE;
  
void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 0;
  data.j1PotY = 0;
  data.j1Button = 1;
  data.j2PotX = 0;
  data.j2PotY = 0;
  data.j2Button = 1;
  data.pot1 = 1;
  data.pot2 = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
}

void printData()
{
  // Print the data in the Serial Monitor
  Serial.print("j1PotX: ");
  Serial.print(data.j1PotX);
  Serial.print("; j1PotY: ");
  Serial.print(data.j1PotY);
  Serial.print("; j1Button: ");
  Serial.print(data.j1Button);
  Serial.print("; j2PotX: ");
  Serial.print(data.j2PotX);
  Serial.print("; j2PotY: ");
  Serial.print(data.j2PotY);
  Serial.print("; j2Button: ");
  Serial.print(data.j2Button);
  Serial.print("; pot1: ");
  Serial.print(data.pot1);
  Serial.print("; pot2: ");
  Serial.print(data.pot2);
  Serial.print("; tSwitch1: ");
  Serial.print(data.tSwitch1);
  Serial.print("; tSwitch2: ");
  Serial.print(data.tSwitch2);
  Serial.print("; button1: ");
  Serial.print(data.button1);
  Serial.print("; button2: ");
  Serial.print(data.button2);
  Serial.print("; button3: ");
  Serial.print(data.button3);
  Serial.print("; button4: ");
  Serial.println(data.button4); 
}