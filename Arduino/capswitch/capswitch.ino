
#include <CapacitiveSensor.h>
int ledPin = 13;
long avg1[8];
long runtotal[8];
int cutoff = 3;
long total[8];
long avg[8];
long loops = 0;
/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */


CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,8);        // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil

void setup()                    
{
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   cs_4_6.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 2 - just as an example
   Serial.begin(9600);
   pinMode(ledPin, OUTPUT);
   pinMode(12, OUTPUT);
   avg[1] = 100;
   avg[2] = 100;
 
}

void loop()                    
{
    long start = millis();
    total[1] =  cs_4_2.capacitiveSensor(30);
    total[2] =  cs_4_6.capacitiveSensor(30);
    //long total3 =  cs_4_8.capacitiveSensor(30);

    //Serial.print(millis() - start);        // check on performance in milliseconds
    //Serial.print("\t");                    // tab character for debug windown spacing
    for (int i = 1; i < 3; i++){
       if(total[i] < cutoff * avg[i])                    // autocalibrate
          runtotal[i] += total[i];
       else
          runtotal[i] += avg[i];
       if(loops == 100){
          for (int j = 1; j < 3; j++){
            avg[j] = round(runtotal[j] / 100);
            runtotal[j] = 0;
            loops = -1;
            Serial.print(j);
            Serial.print("\t");
            Serial.print(avg[j]);
            Serial.print("\n");
            }
          }
       //Serial.print(total[1]);                  // print sensor output 1
       //Serial.print("\t");
       //Serial.print(avg1);
       //Serial.print("\n");
       //Serial.print(total[2]);                  // print sensor output 2
       //Serial.print("\n");
       //Serial.println(total3);                // print sensor output 3
       }
    if (total[1] > avg[1] * cutoff)
       digitalWrite(ledPin, HIGH);
    else
       digitalWrite(ledPin, LOW);
  
    if (total[2] > avg[2] * cutoff)
       digitalWrite(12, HIGH);
    else
       digitalWrite(12, LOW);
 
    
    loops++;   
    delay(10);
    // arbitrary delay to limit data to serial port 
}
