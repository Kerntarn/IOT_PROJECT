
#include <ESP8266WiFi.h>
// #include <BlynkSimpleEsp8266.h>
#include "MQ135.h"

#define MQ_135 A0
#define LED D5
#define DOUT_MQ D3

float analysis(int);

void setup()
{
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(MQ_135, INPUT);
  pinMode(DOUT_MQ, INPUT);
}

void loop()
{
  // Blynk.run();
  // timer.run();
  // MQ135 gasSensor = MQ135(A0);
  // float air_quality = gasSensor.getPPM();
  // Serial.print("Air Quality: ");  
  // Serial.print(air_quality);
  // Serial.println("  PPM");   
  // Serial.println();
  int aqi = analogRead(A0); 
  delay(1000); 
  Serial.print("Air Quality Index: ");
  Serial.print(aqi, DEC); 
  Serial.print(" PPM");

  if(aqi < 500){
    Serial.println(", Fresh Air");
    } else if(aqi > 500 && aqi <= 1000){
      Serial.println(", Poor Air");
    } else if(aqi > 1000){
      Serial.println(", Very Poor"); 
      }
}

// float analysis(int adc){
//   float slope = -0.2884453282;
//   float A = 2.720027932;
//   float Rseries = 1000;
//   float V_Rseries = ((float)adc*5)/1023;
//   Rs = ((5-V_Rseries)/V_Rseries)*Rseries;
//   float R0 = 1809.52;
//   /*
//    * R0 คือ ค่าความต้านทานเซ็นเซอร์ขณะที่ Ammonia เท่ากับ 100 ppm ในอากาศที่สะอาด
//    * ควรปรับจูนด้วยการสอบเทียบกับเซ็นเซอร์มาตรฐาน
//    */
//   float Y = Rs/R0;
//   float Methane_gas = pow(10,(log10(Y/A)/slope));
//   return Methane_gas;
// }
