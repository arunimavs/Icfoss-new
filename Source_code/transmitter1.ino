TRANSMITTER 1

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <EEPROM.h>
//int a = 12345;

int l1=0;
unsigned long int time1=0,time2=0,time;//time1 and time2 to measure the time delay of leakage


const int pulse =2;//port for flow sensor
int ps1,ps,pulsec=0,kp;
int flag1=0,flag2=0,flag=0;//flag for electrode 1 and 2
int sensorValue1, sensorValue2;//electrode 1 and 2

int msg[1];
RF24 radio(9,10);
int sc=55,sb=55;
const uint64_t pipe = 0xE8E8F3F3E1LL;//pipe address for NRF
void setup(void){
  Serial.begin(9600);
  pinMode(pulse, INPUT);      
  
  kp=0;
  radio.begin();
  radio.openWritingPipe(pipe);//writing it on to NRF
}
void loop(void){

  sensorValue1 = analogRead(A0);//electrode 1
  sensorValue2= analogRead(A1);//electrode 2

  Serial.print("sensorValue1");

  Serial.print(sensorValue1);
  Serial.print("sensorValue2");
  Serial.print(sensorValue2);
  Serial.println("");

if(sensorValue2<400||sensorValue1<400)//if electrodes are wet
  {
    if(flag==0)
    {
      if(sensorValue1<500&&flag1==0)//if electrode1 is wet
      {
        time1=millis();//time measure of electrode 1
        flag1=1;
      }
      if(sensorValue2<500&&flag2==0)//if electrode 2 is wet
      {
        time2=millis();//time measure for electrode 2
        flag2=1;
      }
    

      if((flag1==1&&flag2==1)||(flag2==1))//leakage confirmed
      {
        time=time2-time1;

        if(time<15000)//leakage confirming time delay
        {
          l1=1;
        }

        flag=1;
      }
     
    }
  }



  ps = digitalRead(pulse);
  if(ps==1)
    while(ps==1)
      ps = digitalRead(pulse);//read digital high
  if(ps==0)
    while(ps==0)
      ps = digitalRead(pulse);//read digital low

  pulsec++;
  if(pulsec==100)//flowrate threshold
  {
    pulsec=0;  
    kp++;//flowrate value
    EEPROM.write(1, kp);
    Serial.println(kp);

    Serial.println("kp");
    delay(1000);
    
    String theMessage = "";
   
    theMessage.concat("*A");

    theMessage.concat(kp);
    theMessage.concat(",");//*Avalue, differentiate flow rate

    theMessage.concat(l1);
    theMessage.concat("#");//,value# diffrentiate leak value

    //write data on to NRF
    
    int messageSize = theMessage.length();
    for (int i = -1; i < messageSize; i++) {
      int charToSend[1];
      charToSend[0] = theMessage.charAt(i);//char by char
      radio.write(charToSend,1);
    }   
    msg[0] = 2; 
    radio.write(msg,1);
  
  }
  }
