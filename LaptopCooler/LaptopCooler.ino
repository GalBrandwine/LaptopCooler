// (c) Michael Schoeffler 2017, http://www.mschoeffler.de
#include "FastLED.h"
#define DATA_PIN 3
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 38
#define BRIGHTNESS 96
CRGB leds[NUM_LEDS];

#include <EEPROM.h>

/* the current address in the EEPROM (i.e. which byte we're going to write to next) */
int addr = 0;
int MAX_TEMP = -1;
int addr_MIN_TEMP = 1;
int MIN_TEMP = 0;

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
// int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

void setup()
{
    Serial.begin(9600);
    dht.begin();

    delay(3000);                                                                                     // initial delay of a few seconds is recommended
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // initializes LED strip
    FastLED.setBrightness(BRIGHTNESS);                                                               // global brightness
    showProgramCleanUp(100);

    MAX_TEMP = EEPROM.read(addr);
    Serial.print("MAX_TEMP from EEEPROM: ");
    Serial.println(MAX_TEMP);
    // MIN_TEMP = EEPROM.read(addr_MIN_TEMP);
    Serial.print("MIN_TEMP from EEEPROM: ");
    Serial.println(MIN_TEMP);
}

// switches off all LEDs
void showProgramCleanUp(long delayTime)
{
    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
    delay(delayTime);
}

// switches on all LEDs. Each LED is shown in random color.
// delayTime: indicates for how long LEDs are switched on.
void showProgramColorByTemp(float temp, long delayTime)
{
    Serial.print("MAX_TEMP: ");
    Serial.println(MAX_TEMP);
    int mapped = map(int(temp), MIN_TEMP, MAX_TEMP, 160, 255);
    Serial.print("mapped: ");
    Serial.println(mapped);

    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds[i] = CHSV(mapped, 255, 255); // hue, saturation, value
    }
    FastLED.show();
    delay(delayTime);
}

// main program
void loop()
{
    delay(2000);
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp = dht.readTemperature();

    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    temp = round(temp);
    if (temp > MAX_TEMP)
    {
        MAX_TEMP = temp;
        EEPROM.write(addr, MAX_TEMP);
        Serial.print("Reached to new HIGEST temp, Saving: ");
        Serial.println(MAX_TEMP);
    }
    // if (temp < MIN_TEMP)
    // {
    //     MIN_TEMP = temp;
    //     EEPROM.write(addr_MIN_TEMP, MIN_TEMP);
    //     Serial.print("Reached to new LOWEST temp, Saving: ");
    //     Serial.println(MIN_TEMP);
    // }
    showProgramColorByTemp(temp, 100);
}
