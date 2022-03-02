#include <WiFi.h>
#include <Artnet.h>

char *SSID = "Silva-WIFI";
char *PASSWORD = "silvaFTW";

// connect to wifi â€“ returns true if successful or false if not
bool ConnectWifi(void)
{
  bool state = true;
  int i = 0;

  WiFi.begin(SSID, PASSWORD);
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
    Serial.println(SSID);
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

void setup()
{
  Serial.begin(115200);
  ConnectWifi();
  arnetSetup();
}

void loop()
{
  artnetParse();
  if ((WiFi.status() == WL_CONNECTED))
  { // Check the current connection status
    // Serial.println("data received" + users);
    // delay(10000);
  }
  // Serial.println(value);
}
