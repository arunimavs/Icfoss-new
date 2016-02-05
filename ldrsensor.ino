/* Photocell simple testing sketch. 
     
    Connect one end of the photocell to 5V, the other end to Analog 0.
    Then connect one end of a 10K resistor from Analog 0 to ground 
    Connect LED from pin 11 through a resistor to ground 
    For more information see http://learn.adafruit.com/photocells */
     
    int ldrp= 0;     // the cell and 10K pulldown are connected to a0
    int ldrr;     // the analog reading from the sensor divider
    int LEDpin = 11;          // connect Red LED to pin 11 (PWM pin)
    int LEDb;        // 
    void setup(void) {
      // We'll send debugging information via the Serial monitor
      Serial.begin(9600);   
    }
     
    void loop(void) {
      ldrr= analogRead(ldrr);  
     
      Serial.print("Analog reading = ");
      Serial.println(ldrr);     // the raw analog reading
     
      // LED gets brighter the darker it is at the sensor
      // that means we have to -invert- the reading from 0-1023 back to 1023-0
      ldrr = 1023 - ldrr;
      // map 0-1023 to 0-255 since thats the range analogWrite uses
      LEDbrightness = map(ldrr, 0, 1023, 0, 255);
      analogWrite(LEDpin, LEDb);
     
      delay(100);
    }
