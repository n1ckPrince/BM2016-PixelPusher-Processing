#include <CapacitiveSensor.h>
#include "FastLED.h"
#define NUM_LEDS 150
#define datapin  8
CRGB ledsA[NUM_LEDS];
float cutoff = 4.5;
long runtotal[8];
long bruisetime[8];
long bruising = 10000;
long total[8];
long avg[8];
long starttime[8];
int waslow[8];
int loops = 0;
int numstops = round(255/NUM_LEDS);
float ihue = 0;
float istep = 0.01;
int idelay = 10;
int s[8];
int ssum = 0;
int longloops = 1; 

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
   for (int k = 12; k < 14; k++)
      pinMode(k, OUTPUT);
   avg[0] = 100;
   avg[1] = 100;

   // from fast LED
   LEDS.setBrightness(20);
   LEDS.addLeds<WS2811, datapin, GRB>(ledsA, NUM_LEDS);
   memset(ledsA, 0,  NUM_LEDS * sizeof(struct CRGB));
   for(int i = 0; i < NUM_LEDS; i++) 
     ledsA[i] = CRGB( 127, 127, 127);
   for(int i = NUM_LEDS - 4; i < NUM_LEDS; i++)   // will have to work with this on second string
     ledsA[i] = CRGB::Black;
     
   LEDS.show();  
     
 

 
}

void loop()                    
{
    float cut;
    
    
    total[0] =  cs_4_2.capacitiveSensor(30);
    total[1] =  cs_4_6.capacitiveSensor(30);
    //long total3 =  cs_4_8.capacitiveSensor(30);

    //Serial.print(millis() - start);        // check on performance in milliseconds
    //Serial.print("\t");                    // tab character for debug windown spacing
    for (int i = 0; i < 2; i++){
       cut = (float)avg[i] * cutoff * .5;
       if(total[i] < (long)cut && total[i] > 30)                    // autocalibrate
          runtotal[i] += total[i];
       else
          runtotal[i] += avg[i];
       if(loops == 100){
          for (int j = 0; j < 2; j++){
            avg[j] = round(runtotal[j] / 100);
            runtotal[j] = 0;
            loops = -1;
            longloops++;
            if(longloops % 50 == 0){
                avg[0] = 100;
                avg[1] = 100;
                longloops = 1;
                }
 
            //Serial.print(j);
            //Serial.print("\t");
            //Serial.print(avg[j]);
            //Serial.print("\n");
            }
          }
       //Serial.print(total[0]);                  // print sensor output 1
       //Serial.print("\t");
       //Serial.print(avg1);
       //Serial.print("\n");
       //Serial.print(total[1]);                  // print sensor output 2
       //Serial.print("\n");
       //Serial.println(total[2]);                // print sensor output 3
       }
    ssum = 0;   
    for (int k = 0; k < 2; k++){
      cut = (float)cutoff * avg[k];
      if (total[k] > (float)cut){
        digitalWrite(k + 12, HIGH);
        s[k] = 1;
        ssum += pow(2, k);
        if(waslow[k] == 1){
          starttime[k] = millis();
          waslow[k] = 0;
        }
        
        }
      else{
        digitalWrite(k +12, LOW);
        s[k] = 0;
        waslow[k] = 1;
        }      
      }  
    loops++;   
    delay(0);
     
    ledlighting();
    // arbitrary delay to limit data to serial port 
}

void ledlighting(){
  long timenow;
  timenow = millis();
  //Serial.print(ssum);
  //Serial.print("\t");  
  if (ssum == 0){
    //Serial.print("return");
    //return;
    ssum = 0;
    }
  for(int i = 0; i < NUM_LEDS; i++) 
    ledsA[i] = CRGB(127, 127, 127);
  for( int k = 0; k < 2; k++){
      if( bruisetime[k] != 0){
          if( timenow - bruisetime[k] < bruising){
             for( int l = 37 * k; l < 37 * (k + 1); l++)
                ledsA[l] = CRGB( 0, 0, 255);  
             }
          else
             bruisetime[k] = 0;
             
          }
       }
  if(ssum & 0x01){
      setsegment( 0, 37, 0);
      }
  if(ssum & 0x02){
      setsegment( 37, 74, 1);
      }
 if(loops % 10 == 0){
   Serial.print(ssum);
   Serial.print("\t");
   Serial.print(avg[0]);
   Serial.print("\t");
   Serial.print(avg[1]);
   Serial.print("\t");
   Serial.print(total[0]);
   Serial.print("\t");
   Serial.print(total[1]);
   Serial.print("\t");
   Serial.print(longloops);
   Serial.print("\n");
   
   }  
 LEDS.show();
/*    ledsA[i] = CHSV(i*numstops+ihue, 255, 255);
    ihue += istep;
    if(ihue>= 255){
      ihue=0; 
      }
    }
  }
  else{
      for(int i = 0; i < NUM_LEDS; i++) {
    ledsA[i] = CHSV(i*numstops-ihue, 255, 255);
    //ledsA[i] = CHSV(numstops * i, 255, 255);
    ihue += istep;
    if(ihue>= 255){
     ihue=0; 
    }
  }
  }
j++;  
if (j > NUM_LEDS * 2)
   j = 0;  
*/   
delay(idelay); 

}  
  
void setsegment(int i, int j, int segnum){
  long timenow, timediff;
  int l;                                     // fade r, g, b, c;
  timenow = millis();
  timediff = timenow - starttime[segnum];
//Serial.print(i);
//Serial.print("\t");  
   
//Serial.print(j);
//Serial.print("\t");  

//Serial.print(segnum);
//Serial.print("\t");  
  
//Serial.print(timediff);
//Serial.print("\n");  
  
    
  
       ssum -= pow(2, segnum);  
       bruisetime[segnum] = timenow;
       if( timediff < 300){
         
         for(l = i; l < j; l++)
            ledsA[l] = CRGB( 0, 0, 255);
/*          Serial.print("<400");
          c = 0;  fade
          for(l = i; l < i + 8; l++){
            r = 127 - c;
            g = 127 - c;
            b = 127 + c; 
            ledsA[l] = CRGB( r, g, b);
            c += 16;
            }           
          c = 0;
          for(l = j - 8; l < j; l++){
            r = c;
            g = c;
            b = 255 - c; 
            ledsA[l] = CRGB( r, g, b);
            c += 16;
            } */          
         return;
         }

       if( timediff < 600){
          for(l = i; l < j; l++)
             ledsA[l] = CRGB( 25, 0, 230);
/*          c = 0;    fade
          for(l = i; l < i + 8; l++){
            r = 127 - (c * 12);
            g = 127 - (c * 16);
            b = 127 + (c * 12); 
            ledsA[l] = CRGB( r, g, b);
            c++;
            }           
          c = 0;
          for(l = j - 8; l < j; l++){
            r = 25 + (c * 15);
            g = (c * 16);
            b = 230 - ( c * 14); 
            ledsA[l] = CRGB( r, g, b);
            c++;
            }     */      
          return;
          }

       if( timediff < 900){
         for(l = i; l < j; l++)
            ledsA[l] = CRGB( 50, 0, 205);
         return;
         }

       if( timediff < 1200){
         for(l = i; l < j; l++)
            ledsA[l] = CRGB( 75, 0, 180);
         return;
         }


       if( timediff < 1500){
         for(l = i; l < j; l++)
            ledsA[l] = CRGB( 100, 0, 155);
         return;
         }


        if( timediff < 1800){
         for(l = i; l < j; l++)
            ledsA[l] = CRGB( 125, 0, 130);
         return;
         }
       
       if( timediff < 2100){
         for(l = i; l < j; l++)
            ledsA[l] = CRGB( 150, 0, 105);
         //Serial.print("<400");
         return;
         }

       if( timediff < 2400){
          for(l = i; l < j; l++)
             ledsA[l] = CRGB( 175, 0, 80);
         return;
         }

       if( timediff < 2700){
         for(l = i; l < j; l++)
            ledsA[l] = CRGB( 200, 0, 55);
         return;
         }

       if( timediff < 3000){
         for(l = i; l < j; l++)
            ledsA[l] = CRGB( 225, 0, 25);
         return;
         }
         
       if( timediff >= 3000){
         for(l = i; l < j; l++)
            ledsA[l] = CRGB( 255, 0, 0);
         ssum += pow(2, segnum); 
         return;
         }
    
}
