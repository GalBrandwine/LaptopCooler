#include <EEPROM.h>

/* the current address in the EEPROM (i.e. which byte we're going to write to next) */
int addr = 0;
int MAX_TEMP = 1;
int addr_MIN_TEMP = 1;
int MIN_TEMP = 26;

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

    MAX_TEMP = EEPROM.read(addr);
    Serial.print("MAX_TEMP from EEEPROM: ");
    Serial.println(MAX_TEMP);
    EEPROM.write(addr_MIN_TEMP, MIN_TEMP);
    MIN_TEMP = EEPROM.read(addr_MIN_TEMP);
    Serial.print("MIN_TEMP from EEEPROM: ");
    Serial.println(MIN_TEMP);
}

// main program
void loop()
{
}
