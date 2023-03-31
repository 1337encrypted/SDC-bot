#include "GLOBALS.h"

void setup() 
{
  Serial.begin(115200);
  debugln("BEGIN");
  radio.begin();
  radio.openReadingPipe(0, pipe);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  // Use a channel unlikely to be used by Wifi, Microwave ovens etc
  radio.setChannel(124);
  radio.startListening(); //  Set the module as receiver

  slider.attach(sliderPin);

  resetData();
}
void loop() 
{
  // Check whether there is data to be received
  if (radio.available()) 
  {
    radio.read(&data, sizeof(dataPackage)); // Read the whole data and store it into the 'data' structure

    leftMotor.speed = rightMotor.speed = data.pot1;

    if(!data.button1)
    {
      leftMotor.motorStatus = leftMotor.motorStates::BACK;
    }
    else if(!data.button2)
    {
      leftMotor.motorStatus = leftMotor.motorStates::FRONT;
    }
    else
    {
      leftMotor.motorStatus = leftMotor.motorStates::STOP;
    }

    if(!data.button4)
    {
      rightMotor.motorStatus = rightMotor.motorStates::BACK;
    }
    else if(!data.button3)
    {
      rightMotor.motorStatus = rightMotor.motorStates::FRONT;
    }
    else
    {
      rightMotor.motorStatus = rightMotor.motorStates::STOP;
    }

    if(data.tSwitch1)
    {
      servoStatus = servoStates::SERVOBACK;
    }
    if(!data.tSwitch1)
    {
      servoStatus = servoStates::SERVOFRONT;
    }

    switch(leftMotor.motorStatus)
    {
      case leftMotor.motorStates::FRONT:
      leftMotor.front();
      break;

      case leftMotor.motorStates::BACK:
      leftMotor.back();
      break;

      case leftMotor.motorStates::STOP:
      leftMotor.brake();
      break;
    }

    switch(rightMotor.motorStatus)
    {
      case rightMotor.motorStates::FRONT:
      rightMotor.front();
      break;

      case rightMotor.motorStates::BACK:
      rightMotor.back();
      break;

      case rightMotor.motorStates::STOP:
      rightMotor.brake();
      break;
    }

    switch(servoStatus)
    {
      case servoStates::SERVOFRONT:
      slider.write(90);
      break;

      case servoStates::SERVOBACK:
      slider.write(0);
      break;
    }
  }
  lastReceiveTime = millis(); // At this moment we have received the data

  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) 
  { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  }



  //printData()
  
}

