// (c) Michael Schoeffler 2017, http://www.mschoeffler.de
#include "FastLED.h"
#define DATA_PIN 3
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 70
#define BRIGHTNESS 96
CRGB leds[NUM_LEDS];


#include <EEPROM.h>

/* the current address in the EEPROM (i.e. which byte we're going to write to next) */
int addr = 0;
int HIGHEST_TEMP_REACHED = -1;
int MIN_TEMP = 28;

/* How to use the DHT-22 sensor with Arduino uno
   Temperature and humidity sensor
*/
//Libraries
#include <DHT.h>;

//Constants
#define DHTPIN 5          // what pin we're connected to
#define DHTTYPE DHT22     // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value


void setup() {
  Serial.begin(9600);
  dht.begin();
  
  delay(3000); // initial delay of a few seconds is recommended
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // initializes LED strip
  FastLED.setBrightness(BRIGHTNESS);// global brightness

  HIGHEST_TEMP_REACHED = EEPROM.read(addr);
  Serial.print("got from EEEPROM: ");Serial.println(HIGHEST_TEMP_REACHED);
}

// switches off all LEDs
void showProgramCleanUp(long delayTime) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(delayTime);
}


// switches on all LEDs. Each LED is shown in collor enterpulated by inputed temp.
// numIterations: indicates how often LEDs are switched on in random colors
// delayTime: indicates for how long LEDs are switched on.
void showProgramRandom(int numIterations, long delayTime) {
  for (int iteration = 0; iteration < numIterations; ++iteration) {
    for (int i = 0; i < NUM_LEDS; ++i) {
      leds[i] = CHSV(random8(),255,255); // hue, saturation, value
    }
    FastLED.show();
    delay(delayTime);
  }
}

// switches on all LEDs. Each LED is shown in random color.
// delayTime: indicates for how long LEDs are switched on.
void showProgramColorByTemp(float temp, long delayTime) {  
  int mapped = map(int(temp),MIN_TEMP,HIGHEST_TEMP_REACHED,160,255);
  
  Serial.print("mapped: ");Serial.println(mapped);
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CHSV(mapped,255,255); // hue, saturation, value
  }
  FastLED.show();
  delay(delayTime);
}

// Shifts a single pixel from the start of strip to the end.
// crgb: color of shifted pixel
// delayTime: indicates how long the pixel is shown on each LED
void showProgramShiftSinglePixel(CRGB crgb, long delayTime) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = crgb;
    FastLED.show();
    delay(delayTime);
    leds[i] = CRGB::Black;
  }
}

// Shifts multiple pixel from the start of strip to the end. The color of each pixel is randomized.
// delayTime: indicates how long the pixels are shown on each LED
void showProgramShiftMultiPixel(long delayTime) {
  for (int i = 0; i < NUM_LEDS; ++i) { 
    for (int j = i; j > 0; --j) {
      leds[j] = leds[j-1];
    }
    CRGB newPixel = CHSV(random8(), 255, 255);
    leds[0] = newPixel;
    FastLED.show();
    delay(delayTime);
  }
}

// main program
void loop() {
  delay(2000);
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp= dht.readTemperature();

  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp);
  Serial.println(" Celsius");

  if (round( temp) > HIGHEST_TEMP_REACHED){
    HIGHEST_TEMP_REACHED = round( temp);
    EEPROM.write(addr, HIGHEST_TEMP_REACHED);
    Serial.print("Reached to new HIGEST temp, Saving, ");Serial.println(HIGHEST_TEMP_REACHED);
  }
//  delay(10000); //Delay 2 sec.

  showProgramColorByTemp(temp, 100); 
  
//  showProgramCleanUp(2500); // clean up
//  showProgramRandom(100, 100); // show "random" program
  
//  showProgramCleanUp(2500); // clean up
//  showProgramRandom(10, 1000); // show "random" program
//  
//  showProgramCleanUp(2500); // clean up
//  showProgramShiftSinglePixel(CRGB::Blue, 250); // show "shift single pixel program" with blue pixel
//  
//  showProgramCleanUp(2500); // clean up
//  showProgramShiftSinglePixel(CRGB::Maroon, 100); // show "shift single pixel program" with maroon pixel
//  
//  showProgramCleanUp(2500); // clean up
//  showProgramShiftMultiPixel(500); // show "shift multi pixel" program
//  
//  showProgramCleanUp(2500); // clean up
//  showProgramShiftMultiPixel(50); // show "shift multi pixel" program
}
