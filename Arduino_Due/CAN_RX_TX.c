
// Required libraries
#include "variant.h"
#include <due_can.h>

//Leave defined if use native port, comment if using programming port
//#define Serial SerialUSB

void setup()
{

  Serial.begin(115200);
  
  // Initialize CAN0, Set the proper baud rates
  Can0.begin(CAN_BPS_500K);
  
  Can0.watchFor();  
}

void sendData()
{
  CAN_FRAME outgoing;
  outgoing.id = 0x400;
  outgoing.length = 8;
  outgoing.extended = false;
  outgoing.priority = 15; //0-15 lower is higher priority
  
  outgoing.data.byte[0] = 0x01;
  outgoing.data.byte[1] = 0x02;
  outgoing.data.byte[2] = 0x03;
  outgoing.data.byte[3] = 0x04;
  outgoing.data.byte[4] = 0x05;
  outgoing.data.byte[5] = 0x06;
  outgoing.data.byte[6] = 0x07;
  outgoing.data.byte[7] = 0x08;

  Can0.sendFrame(outgoing);
}

void loop(){
  CAN_FRAME incoming;
  static unsigned long lastTime = 0;

  if (Can0.available() > 0) {
      Can0.read(incoming);
      Serial.print("CAN message received = ");
      Serial.print("ID : ");
      Serial.print(incoming.id, HEX); Serial.print(" ");
      Serial.print("| Data : ");
      Serial.print(incoming.data.byte[0]); Serial.print(" ");   
      Serial.print(incoming.data.byte[1]); Serial.print(" ");
      Serial.print(incoming.data.byte[2]); Serial.print(" ");
      Serial.print(incoming.data.byte[3]); Serial.print(" ");
      Serial.print(incoming.data.byte[4]); Serial.print(" ");
      Serial.print(incoming.data.byte[5]); Serial.print(" ");
      Serial.print(incoming.data.byte[6]); Serial.print(" ");
      Serial.println(incoming.data.byte[7]);

   }

  if ((millis() - lastTime) > 500) 
  {
     lastTime = millis();
     sendData();    
  }
}

