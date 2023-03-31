#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

#define CE_PIN 5
#define CSN_PIN 6

//Joystick pin declaration and object declaration
#define joyXaxisPin1 A1           // Joystick X axis 1
#define joyYaxisPin1 A0           // Joystick Y axis 1
#define joyXaxisPin2 A2           // Joystick X axis 1
#define joyYaxisPin2 A3           // Joystick Y axis 1
#define joyBtn1 0
#define joyBtn2 1                 // Toggle switch 1

//Toggle switch pins declaration and object
#define tglPin1 7                 // Toggle switch 1
#define tglPin2 4                 // Toggle switch 1

//Potentiometer pins declaration and object
#define potPin1 A7                // Toggle switch 1
#define potPin2 A6                // Toggle switch 1

//Pushbuttons pin declaration and object declaration
#define btnPin1 8                 // Button 1
#define btnPin2 9                 // Button 2
#define btnPin3 2                 // Button 3
#define btnPin4 3                 // Button 4


RF24 radio(CE_PIN, CSN_PIN);   // nRF24L01 (CE, CSN)
const uint64_t pipe = 0xd24822e8; // This is the transmit pipeline

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
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

Data_Package data; //Create a variable with the above structure

void setup() 
{
  Serial.begin(115200);

  // Call this function if you need to get the IMU error values for your module
  //calculate_IMU_error();
  
  // Initiate the radio object
  radio.begin();
  radio.setAutoAck(false);
  // Set the speed of the transmission to the lowest highest is RF24_2MBPS
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipe);
  // Set the transmit power to lowest available to prevent power supply related issues
  radio.setPALevel(RF24_PA_LOW);
  // Use a channel unlikely to be used by Wifi, Microwave ovens etc
  radio.setChannel(124);
  
  // Activate the Arduino internal pull-up resistors
  // pinMode(joyXaxisPin1, OUTPUT);
  // pinMode(joyYaxisPin1, OUTPUT);
  pinMode(joyBtn1, INPUT_PULLUP);
  // pinMode(joyXaxisPin2, OUTPUT);
  // pinMode(joyYaxisPin2, OUTPUT);
  pinMode(joyBtn2, INPUT_PULLUP);
  pinMode(potPin1, INPUT);
  pinMode(potPin2, INPUT);
  pinMode(tglPin1, INPUT_PULLUP);
  pinMode(tglPin2, INPUT_PULLUP);
  pinMode(btnPin1, INPUT_PULLUP);
  pinMode(btnPin2, INPUT_PULLUP);
  pinMode(btnPin3, INPUT_PULLUP);
  pinMode(btnPin4, INPUT_PULLUP);
  
  // Set initial default values
  data.j1PotX = 0; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  data.j1PotY = 0;
  data.j1Button = 1;
  data.j2PotX = 0;
  data.j2PotY = 0;
  data.j2Button = 1;
  data.pot1 = 0;
  data.pot2 = 0;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
}
void loop() 
{

  // Read all analog inputs and map them to one Byte value

  radio.write(&data, sizeof(Data_Package));

  // Read all analog inputs and map them to one Byte value
  data.j1PotX = map(analogRead(joyXaxisPin1),0,1023,0,255); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.j1PotY = map(analogRead(joyYaxisPin1),0,1023,0,255);
  data.j1Button = digitalRead(joyBtn1);
  data.j2PotX = map(analogRead(joyXaxisPin2),0,1023,0,255);
  data.j2PotY = map(analogRead(joyYaxisPin2),0,1023,0,255);
  data.j2Button = digitalRead(joyBtn2);
  data.pot1 = map(analogRead(potPin1),0,1023,0,255);
  data.pot2 = map(analogRead(potPin2),0,1023,0,255);
  data.tSwitch1 = digitalRead(tglPin1);
  data.tSwitch2 = digitalRead(tglPin2);
  data.button1 = digitalRead(btnPin1);
  data.button2 = digitalRead(btnPin2);
  data.button3 = digitalRead(btnPin3);
  data.button4 = digitalRead(btnPin4);
  // Send the whole data from the structure to the receiver
  radio.write(&data, sizeof(Data_Package));

  //Print the data in the Serial Monitor
  // Serial.print("j1PotX: ");
  // Serial.print(data.j1PotX);
  // Serial.print("; j1PotY: ");
  // Serial.print(data.j1PotY);
  // Serial.print("; j1Button: ");
  // Serial.print(data.j1Button);
  // Serial.print("; j2PotX: ");
  // Serial.print(data.j2PotX);
  // Serial.print("; j2PotY: ");
  // Serial.print(data.j2PotY);
  // Serial.print("; j2Button: ");
  // Serial.print(data.j2Button);
  // Serial.print("; pot1: ");
  // Serial.print(data.pot1);
  // Serial.print("; pot2: ");
  // Serial.print(data.pot2);
  // Serial.print("; tSwitch1: ");
  // Serial.print(data.tSwitch1);
  // Serial.print("; tSwitch2: ");
  // Serial.print(data.tSwitch2);
  // Serial.print("; button1: ");
  // Serial.print(data.button1);
  // Serial.print("; button2: ");
  // Serial.print(data.button2);
  // Serial.print("; button3: ");
  // Serial.print(data.button3);
  // Serial.print("; button4: ");
  // Serial.println(data.button4); 

}
