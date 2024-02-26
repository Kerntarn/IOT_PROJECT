#include <MQ135.h>
#include <Wire.h>
#include "SH1106.h"
#include "SH1106Ui.h"
/*  MQ135 gas sensor
    Datasheet can be found here: https://www.olimex.com/Products/Components/Sensors/SNS-MQ135/resources/SNS-MQ135.pdf

    Application
    They are used in air quality control equipments for buildings/offices, are suitable for detecting of NH3, NOx, alcohol, Benzene, smoke, CO2, etc

    Original creator of this library: https://github.com/GeorgK/MQ135
*/

#define PIN_MQ135 A0
// Pin definitions for I2C
#define OLED_SDA    D2  // pin 14
#define OLED_SDC    D1  // pin 12
#define OLED_ADDR   0x3C

#define Fan D5
#define Buzzer D6
#define BLYNK_TEMPLATE_ID "TMPL6mE3M-7EZ"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "EA0ouLHD0c2tPhhaHTYgQIk4-tIMt1PS"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

char ssid[] = "";
char pass[] = "";

float rzero;
float resistance;
float ppm;

BlynkTimer timer;

MQ135 mq135_sensor(PIN_MQ135);//223.63   187.69

SH1106   display(OLED_ADDR, OLED_SDA, OLED_SDC);    // For I2C
SH1106Ui ui     ( &display );

BLYNK_WRITE(V0){
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  // Update state
  Blynk.virtualWrite(V1, value);
  digitalWrite(Fan, value);
  Serial.print("Status: ");
  value == 1 ? Serial.println("ON") : Serial.println("OFF");
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// // This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V4, mq135_sensor.getPPM());
}

bool msOverlay(SH1106 *display, SH1106UiState* state) {
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(128, 0, String(millis()));
  return true;
}

bool drawFrame1(SH1106 *display, SH1106UiState* state, int x, int y) {
  // draw an xbm image.
  // Please note that everything that should be transitioned
  // needs to be drawn relative to x and y

  // if this frame need to be refreshed at the targetFPS you need to
  // return true
  // display->drawXbm(0, y + 30, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  return false;
}

bool drawFrame2(SH1106 *display, SH1106UiState* state, int x, int y) {
  // Demonstrates the 3 included default sizes. The fonts come from SH1106Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(0,0, "Air Quality Index : " + String(ppm);
  if(ppm > 5000){     //Unhealthy as fuck
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(0 + x,10 + y, "Hazardous");
    display->drawString(0 + x,20 + y, "Fan is ON");
  }
  else if(ppm > 1500){  //Unhealthy
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(0 + x,10 + y, "Unhealthy");
    display->drawString(0 + x,20 + y, "Fan is ON");
  }
  else if(ppm > 300){ //Moderate
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(0 + x,10 + y, "Moderate");
    display->drawString(0 + x,20 + y, "Fan is ON");
  }
  else{ //Very Good
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(0 + x,10 + y, "Very good");
    display->drawString(0 + x,20 + y, "Fan is OFF");
  }
  return false;
}

bool drawFrame3(SH1106 *display, SH1106UiState* state, int x, int y) {
  // Text alignment demo
  display->setFont(ArialMT_Plain_10);

  // // The coordinates define the left starting point of the text
  // display->setTextAlignment(TEXT_ALIGN_LEFT);
  // display->drawString(0 + x, 0 + y, "Left aligned (0,0)");

  // The coordinates define the center of the text
  
  display->drawString(0, 0, "Center aligned (64,22)");
  display->setTextAlignment(TEXT_ALIGN_RIGHT);

  // // The coordinates define the right end of the text
  // display->setTextAlignment(TEXT_ALIGN_RIGHT);
  // display->drawString(100 + x, 33 + y, "Right aligned (128,33)");
  return false;
}

bool drawFrame4(SH1106 *display, SH1106UiState* state, int x, int y) {
  // Demo for drawStringMaxWidth:
  // with the third parameter you can define the width after which words will be wrapped.
  // Currently only spaces and "-" are allowed for wrapping
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawStringMaxWidth(0 + x, 10 + y, 128, "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore.");
  return false;
}

// how many frames are there?
int frameCount = 1;
// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
bool (*frames[])(SH1106 *display, SH1106UiState* state, int x, int y) = {drawFrame2};

// bool (*overlays[])(SH1106 *display, SH1106UiState* state)             = { msOverlay };
// int overlaysCount = 1;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_MQ135, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Fan, OUTPUT);

  ui.setTargetFPS(60);

  // ui.setActiveSymbole(activeSymbole);
  // ui.setInactiveSymbole(inactiveSymbole);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(BOTTOM);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Add frames
  ui.setFrames(frames, frameCount);

  // Add overlays
  // ui.setOverlays(overlays, overlaysCount);

  // Inital UI takes care of initalising the display too.
  ui.init();

  display.flipScreenVertically();

  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // // You can also specify server:
  // //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  // //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Setup a function to be called every second
  // timer.setInterval(1000L, myTimerEvent);
  // Serial.print("Your local IP is: ");
  // Serial.println(WiFi.localIP());
}

void loop() {

  // Blynk.run();
  // timer.run();

  rzero = mq135_sensor.getRZero();
  resistance = mq135_sensor.getResistance();
  ppm = mq135_sensor.getPPM();
  Serial.print("MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\t Harmful gases PPM: ");
  Serial.print(ppm);
  Serial.print(" ppm");
  Serial.print("\t\t\t");
  Serial.println(analogRead(PIN_MQ135));

  if(ppm > 5000){     //Unhealthy as fuck
    digitalWrite(Fan, 1);
    digitalWrite(Buzzer, 1);
    delay(200);
    digitalWrite(Buzzer, 0);
    delay(200);
  }
  else if(ppm > 1500){  //Unhealthy
    digitalWrite(Fan, 1);
    digitalWrite(Buzzer, 1);
    delay(400);
    digitalWrite(Buzzer, 0);
    delay(1000);
  }
  else if(ppm > 300){ //Moderate
    digitalWrite(Fan, 1);
    delay(200);
  }
  else{ //Very Good
    digitalWrite(Fan, 0);
    delay(200);
  }

  int remainingTimeBudget = ui.update();
  // if (remainingTimeBudget > 0) {

  //   delay(remainingTimeBudget);
  // }
}
