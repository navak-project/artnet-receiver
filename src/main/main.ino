#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArtnetWifi.h>
#include <FastLED.h>

// Wifi settings - be sure to replace these with the WiFi network that your computer is connected to

const char *ssid = "Silva-WIFI";
const char *password = "silvaFTW";

// LED Strip
const int numLeds = 400;                  // Change if your setup has more or less LED's
const int numberOfChannels = numLeds * 3; // Total number of DMX channels you want to receive (1 led = 3 channels)
#define DATA_PIN 19                       // The data pin that the WS2812 strips are connected to. 18
#define DATA_PIN2 18                       // The data pin that the WS2812 strips are connected to. 18
CRGB leds[numLeds];

// Artnet settings
ArtnetWifi artnet;
const int startUniverse = 250;

const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
bool universesReceived[maxUniverses];
//bool sendFrame = 1;

bool sendFrame = 1;
int previousDataLength = 0;

// connect to wifi – returns true if successful or false if not
boolean ConnectWifi(void)
{
  boolean state = true;
  int i = 0;

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (i > 20)
    {
      state = false;
      break;
    }
    i++;
  }
  if (state)
  {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data)
{
  Serial.println(universe);
  Serial.println(length);
  sendFrame = 1;
  // set brightness of the whole strip
  if (universe == 15)
  {
    FastLED.setBrightness(data[0]);
  }
  // read universe and put into the right part of the display buffer
  for (int i = 0; i < length / 3; i++)
  {
    int led = i + (universe - startUniverse) * (previousDataLength / 3);
    if (led < numLeds)
    {
      leds[led] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);

    }
  }
  previousDataLength = length;
  FastLED.show();
}

//void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
//{
//  sendFrame = 1;
//  // set brightness of the whole strip
//  if (universe == 15)
//  {
//    FastLED.setBrightness(data[0]);
//    FastLED.show();
//  }
//
//  // range check
//  if (universe < startUniverse)
//  {
//    return;
//  }
//  uint8_t index = universe - startUniverse;
//  if (index >= maxUniverses)
//  {
//    return;
//  }
//
//  // Store which universe has got in
//  universesReceived[index] = true;
//
//  for (int i = 0 ; i < maxUniverses ; i++)
//  {
//    if (!universesReceived[i])
//    {
//      sendFrame = 0;
//      break;
//    }
//  }
//
//  // read universe and put into the right part of the display buffer
//  for (int i = 0; i < length / 3; i++)
//  {
//    int led = i + (index * 170);
//    if (led < numLeds)
//    {
//      leds[led] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
//    }
//  }
//
//  if (sendFrame)
//  {
//    FastLED.show();
//    // Reset universeReceived to 0
//    memset(universesReceived, 0, maxUniverses);
//  }
//}

void setup()
{
  Serial.begin(115200);
  ConnectWifi();
  artnet.begin();
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, numLeds);
  FastLED.addLeds<WS2812, DATA_PIN2, GRB>(leds, numLeds);
  // onDmxFrame will execute every time a packet is received by the ESP32
//  memset(universesReceived, 0, maxUniverses);
  
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop()
{
  // we call the read function inside the loop
  artnet.read();
}
