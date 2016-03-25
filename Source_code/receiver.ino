RECEIVER 


#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>  //header file for NRF module
#include <SPI.h>
#include<TESPI.h>        //header file for ESP module
int msg[1];
RF24 radio(9, 53);      //Atmega pins for NRF reception
TESPI esp(Serial3);
const uint64_t pipe1 = 0xE8E8F3F3E1LL;//pipe 1 
const uint64_t pipe2 = 0xE8E8F5F5E2LL;//pipe 2
int f1 = 0, f2 = 1;


String theMessage = "";
void setup(void) {
  Serial.begin(9600);
  Serial3.begin(9600);
  radio.begin();//start reading the data from NRF transmitter
  esp.setWifiStationConfig("Cygnus", "icfoss123", 1);//wifi address
  esp.loadWifiStationConfig();
}
void loop(void) {
 
  Serial.println("pipe1");

  f1 = 0;
  radio.openReadingPipe(1, pipe1);//read data from pipe 1
  radio.startListening();
  theMessage = "";
  while (f1 == 0)//read until f1 is 0
  {
    if (radio.available()) {
      bool done = false;
      done = radio.read(msg, 1);//read char by char
      char theChar = msg[0];
      if (msg[0] != 2) {
        theMessage.concat(theChar);
      }
      else {
        //communication of data from pipe 1 onto esp
        theMessage.trim();
        int commaA = theMessage.indexOf(','), hashA;
        char leakA;
        String flowA;
        Serial.println(theMessage);
        if (theMessage.indexOf('A') == 1) {
          flowA = theMessage.substring(2, commaA);
          leakA = theMessage.charAt(commaA + 1);
           
          Serial.println("FLOW = " + flowA + " and LEAK = " + leakA);
          hashA = theMessage.indexOf('#');
          
          esp.HTTP_GET("/wm/update.php?cv_no=1&flowrate="+flowA,"smartnodes.in",80);//update data received in cloud
            delay(500);
          if(leakA=='1')  
         {   esp.HTTP_GET("/wm/update.php?lv_no=1&leak=1","smartnodes.in",80);//change parameters in cloud 
           delay(500);
           Serial.println("leak FLOW");
         }
          if(leakA=='0')  
         {   esp.HTTP_GET("/wm/update.php?lv_no=1&leak=0","smartnodes.in",80);//change parameters in cloud
           delay(500);
        Serial.println("leak NO");
           
       }
         
          esp.HTTP_GET("/wm/get_details.php?param=cv&n=1", "smartnodes.in", 80);//collect data from NRF transmitter
          delay(900);


        }
        theMessage = "";

        f1 = 1;
      }
    }
    
  }
  f2 = 0;
  radio.openReadingPipe(1, pipe2);//read pipe 2
  radio.startListening();
  theMessage = "";
  delay(1000);
  Serial.println("pipe2");

  while (f2 == 0)
  {
    if (radio.available()) {
      bool done = false;
      done = radio.read(msg, 1);
      char theChar = msg[0];   //get data character by character 
      if (msg[0] != 2) {
        theMessage.concat(theChar); //data received from 2nd pipe
      }
      else {
        //writing data from pipe  on to ESP
        theMessage.trim();
        int commaB = theMessage.indexOf(','), hashB;
        char leakB;
        String flowB;
        Serial.println(theMessage);
        if (theMessage.indexOf('B') == 1) {
          flowB = theMessage.substring(2, commaB);
          leakB = theMessage.charAt(commaB + 1);
          Serial.println("FLOW = " + flowB + " and LEAK = " + leakB);
          hashB = theMessage.indexOf('#');
           esp.HTTP_GET("/wm/update.php?lv_no=2&leak="+leakB,"smartnodes.in",80);//update leak value in cloud 
                   delay(500);
                   esp.HTTP_GET("/wm/update.php?cv_no=2&flowrate="+flowB,"smartnodes.in",80);//update flow value in cloud
                   delay(500);
                   esp.HTTP_GET("/wm/update.php?lv_no=2&leak="+leakB,"smartnodes.in",80);
          esp.HTTP_GET("/wm/get_details.php?param=cv&n=1", "smartnodes.in", 80);
        }
        theMessage = "";

        f2 = 1;
      }
      
    }
  }
  Serial.println("||" + (int)esp.responseAvailable());//print response of ESP
}

