// (c) Michael Schoeffler 2017, http://www.mschoeffler.de
#include <FastLED.h>
#define DATA_PIN 3
#define LED_TYPE WS2812B
#define COLOR_ORDER EOrder::GRB
#define NUM_LEDS 38
#define BRIGHTNESS 96
CRGB leds[NUM_LEDS];

#include <EEPROM.h>

/* the current address in the EEPROM (i.e. which byte we're going to write to next) */
int addr = 0;
int MAX_TEMP = 26;
int addr_MIN_TEMP = 1;
int MIN_TEMP = MAX_TEMP - 1;

/* How to use the DHT-22 sensor with Arduino uno
   Temperature and humidity sensor
*/
// Libraries
#include <DHT.h>;

// Constants
#define DHTPIN 5          // what pin we're connected to
#define DHTTYPE DHT22     // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

// Variables
//  int chk;
float hum;  // Stores humidity value
float temp; // Stores temperature value

void resetEEPROM()
{
    Serial.println("Before reset: ");
    Serial.print("MAX_TEMP from EEEPROM: ");
    Serial.println(EEPROM.read(addr));
    Serial.print("MIN_TEMP from EEEPROM: ");
    Serial.println(EEPROM.read(addr_MIN_TEMP));

    EEPROM.write(addr, 0);
    EEPROM.write(addr_MIN_TEMP, 0);
}
void initialEEPROMReading()
{
    MAX_TEMP = EEPROM.read(addr);
    Serial.print("MAX_TEMP from EEEPROM: ");
    Serial.println(MAX_TEMP);
    auto tempMin = EEPROM.read(addr_MIN_TEMP);
    if (tempMin == 0)
    {
        Serial.print("Got initial values EEEPROM: ");
        Serial.println(tempMin);

        Serial.print("reseting to default:");
        Serial.println(MIN_TEMP);
        EEPROM.write(addr_MIN_TEMP, MIN_TEMP);
    }
    else
    {
        Serial.print("MIN_TEMP from EEEPROM: ");
        Serial.println(MIN_TEMP);
    }
}
void setup()
{
    Serial.begin(9600);
    dht.begin();

    delay(3000);                                                                                     // initial delay of a few seconds is recommended
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // initializes LED strip
    FastLED.setBrightness(BRIGHTNESS);                                                               // global brightness
    showProgramCleanUp(100);

    // resetEEPROM();
    initialEEPROMReading();
}

// switches off all LEDs
void showProgramCleanUp(long delayTime)
{
    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds[i] = CRGB::HTMLColorCode::Black;
    }
    FastLED.show();
    FastLED.delay(delayTime);
}

// switches off all LEDs
void blinkInColor(CRGB::HTMLColorCode color, long delayTime)
{
    showProgramCleanUp(delayTime);
    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds[i] = color;
    }
    FastLED.show();
    FastLED.delay(delayTime);
    showProgramCleanUp(delayTime);

    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds[i] = color;
    }
    FastLED.show();
    FastLED.delay(delayTime);
    showProgramCleanUp(delayTime);
}

// switches on all LEDs. Each LED is shown in random color.
// delayTime: indicates for how long LEDs are switched on.
void showProgramColorByTemp(int currentTemp, long delayTime)
{
    Serial.println(__FUNCTION__);
    Serial.print("MIN_TEMP: ");
    Serial.println(MIN_TEMP);
    Serial.print("MAX_TEMP: ");
    Serial.println(MAX_TEMP);

    auto hue = map(currentTemp, MAX_TEMP, MIN_TEMP, HUE_RED, HUE_BLUE); // Lower TEMP gets bluer HUE
    Serial.print("mapped: ");
    Serial.println(hue);

    for (int i = 0; i < NUM_LEDS; ++i)
    {
        leds[i].setHue(hue); // = CHSV(); // hue, saturation, value
    }
    FastLED.show();
    FastLED.delay(delayTime);
    Serial.println();
    Serial.println();
}

// main program
void loop()
{
    // Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp = dht.readTemperature();

    // Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");

    if (temp > MAX_TEMP)
    {
        MAX_TEMP = ceil(temp);
        EEPROM.write(addr, MAX_TEMP);
        Serial.print("Reached to new HIGHEST temp, Saving: ");
        Serial.println(MAX_TEMP);
        blinkInColor(CRGB::Red, 300);
    }
    if (temp < MIN_TEMP)
    {
        MIN_TEMP = floor(temp);
        EEPROM.write(addr_MIN_TEMP, MIN_TEMP);
        Serial.print("Reached to new LOWEST temp, Saving: ");
        Serial.println(MIN_TEMP);
        blinkInColor(CRGB::Blue, 300);
    }
    showProgramColorByTemp(int(temp), 2000);
}
