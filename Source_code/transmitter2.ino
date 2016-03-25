
TRANSMITTER 2


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <EEPROM.h>

int l1=0;
unsigned long int time1=0,time2=0,time;//time1 and time2 to measure the time delay of leakage


const int pulse =2;//port for flow sensor
int ps1,ps,pulsec=0,kp;
int flag1=0,flag2=0,flag=0;//flag for electrode 1 and 2
int sensorValue1, sensorValue2;//electrode 1 and 2
String theMessage;
int msg[1];
RF24 radio(9,10);
int sc=55,sb=55;
const uint64_t pipe = 0xE8E8F5F5E2LL;//pipe address 2 for NRF
void setup(void){
  Serial.begin(9600);
  pinMode(pulse, INPUT);      
  

  kp=0;
  radio.begin();
  radio.openWritingPipe(pipe);//write data n to NRF
}
void loop(void){

  ps = digitalRead(pulse);
  if(ps==1)
    while(ps==1)
      ps = digitalRead(pulse);//read digital high value
  if(ps==0)
    while(ps==0)
      ps = digitalRead(pulse);//read digital low value

  pulsec++;
  if(pulsec==100)//to read flow rate threshold
  {
    pulsec=0;  
    kp++;//flowrate
    EEPROM.write(1, kp);
    Serial.println(kp);

    Serial.println("kp");
    delay(1000);
    
     theMessage = "";
   
    theMessage.concat("*B");

    theMessage.concat(kp);
    theMessage.concat(",");//*Bvalue, diffrentiate kp

    theMessage.concat(l1);
    theMessage.concat("#");//,value# diffrentiate l1
    
    //sending data to NRF module
    int messageSize = theMessage.length();
    for (int i = -1; i < messageSize; i++) {
      int charToSend[1];
      charToSend[0] = theMessage.charAt(i);//write char by char
      radio.write(charToSend,1);
    }   
    msg[0] = 2; 
    radio.write(msg,1);
  
  }
  

}



