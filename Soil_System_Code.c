/*

    To use the DEBUG printouts
    comment out the following:
      - #include <BlynkSimpleEthernet.h>
      - #include <Ethernet.h>
      - Blynk.begin(BLYNK_AUTH_TOKEN);
      - everything below Blynk.run();
    and
      - set Serial.begin() to 9600 bodrate

*/

/* CONSTANTS */
#define READ_DELAY 2000 // In Miliseconds // For release make it 5000ms
#define BLYNK_TEMPLATE_ID           "TMPL4DbraaxOU"
#define BLYNK_TEMPLATE_NAME         "метерологична система"
#define BLYNK_AUTH_TOKEN            "pRseYZGUwKEe5FdslHToXxEj-g6Vez8Z"

/* LIBRARIES */
#include <DHT11.h>
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

/* PIN LABELS */
int   INDOOR_DHT11 = 2;
int  OUTDOOR_DHT11 = 4;
int    RAIN_SENSOR = A0;
int    SOIL_SENSOR = A1;

/* DHT11 CLASS DECLARATION */
DHT11 cDHT11_IN(INDOOR_DHT11);
DHT11 cDHT11_OUT(OUTDOOR_DHT11);

/* FUNCTION DECLARATION */
void readDhtInside();
void readDhtOutside();
void readRain();
void readSoil();



void setup() {
  Serial.begin(115200); // 9600|115200
  Blynk.begin(BLYNK_AUTH_TOKEN);
}

void loop() {
  /* Read Sensor Data */
  // sd means Sensor Data
  int sdTempIn, sdTempOut, sdHumidityIn, sdHumidityOut, sdRain, sdSoil;
  readDhtInside(&sdTempIn, &sdHumidityIn);
  readDhtOutside(&sdTempOut, &sdHumidityOut);
  readRain(&sdRain);
  readSoil(&sdSoil);

  /* Communicate Sensor Data to Blynk */
  Blynk.run();
  Blynk.virtualWrite(V1, sdSoil);
  Blynk.virtualWrite(V0, sdRain);
  Blynk.virtualWrite(V2, sdTempIn);
  Blynk.virtualWrite(V3, sdHumidityOut);
  Blynk.virtualWrite(V6, sdTempOut);
  
  delay(READ_DELAY);
}



/* FUNCTION DEFINITION */
void readDhtInside(int* inTemp, int* inHumi)
{
  int inResult = cDHT11_IN.readTemperatureHumidity(*inTemp, *inHumi);

  if (inResult == 0) { // 0 means successful read
    //Serial.print("Temperature Inside:");  // DEBUG //
    //Serial.print(*inTemp);                // DEBUG //
    //Serial.print(", Humidity Inside:");   // DEBUG //
    //Serial.println(*inHumi);              // DEBUG //
  } else { // else read failed
    Serial.println(DHT11::getErrorString(inResult));
  }
}

void readDhtOutside(int* outTemp, int* outHumi)
{
  int outResult = cDHT11_OUT.readTemperatureHumidity(*outTemp, *outHumi);

  if (outResult == 0) { // 0 means successful read
    //Serial.print("Temperature Outside:"); // DEBUG //
    //Serial.print(*outTemp);               // DEBUG //
    //Serial.print(", Humidity Outside:");  // DEBUG //
    //Serial.println(*outHumi);             // DEBUG //
  } else { // else read failed
    Serial.println(DHT11::getErrorString(outResult));
  }
}

void readRain(int* sdRain)
{
  *sdRain = 100-map(analogRead(RAIN_SENSOR), 0, 1023, 0, 100);
  //Serial.print("Rain Percentage: ");  // DEBUG //
  //Serial.println(*sdRain);            // DEBUG //
}

void readSoil(int* sdSoil)
{
  *sdSoil = 100-map(analogRead(SOIL_SENSOR), 0, 1023, 0, 100);
  //Serial.print("Soil Humidity Percentage: "); // DEBUG //
  //Serial.println(*sdSoil);                    // DEBUG //
}