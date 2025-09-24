#include "wifi_network.h"
#include "network-time.h"
#include <WiFiManager.h>

const int   daylightOffset_sec = 3600;
const long  gmtOffset_sec = -3600 * 5;
const char* ntpServer = "pool.ntp.org";
struct tm timeInfo;

// NetworkTime::NetworkTime() {
// }

int16_t NetworkTime::getDaysUntilEvent(time_t eventTime){
  return elapsedDays(eventTime) - elapsedDays(mktime(&timeInfo) + gmtOffset_sec);
}

boolean NetworkTime::getLocalTime(){
  return ::getLocalTime(&timeInfo);
}

void NetworkTime::getNetworkTime() {
  // copyDownloadImage();
  NetworkTime::wifiConnect(ssid, password);

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  NetworkTime::printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

void NetworkTime::printLocalTime() {
  struct tm timeInfo;

  if(!getLocalTime()){
    Serial.println("Failed to obtain time");
    return;
  }

  Serial.println(&timeInfo, "%A, %B %d %Y %H:%M:%S");
}

void NetworkTime::wifiConnect(const char* ssid, const char* pw) {
  WiFiManager wm;
  // wm.resetSettings();

  bool res;
  uint8_t count = 20;

  // Connect to wifi
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // res = wm.autoConnect(ssid, pw);
  res = wm.autoConnect(ssid);

  if (!res) {
    Serial.println("Failed to connect");
    return;
  }

  while (WiFi.status() != WL_CONNECTED && count > 0) {
    count--;
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");

  if (WiFi.status() == WL_CONNECTED) {
    // wifiConnected = true;

    Serial.print("Connected to WiFi at ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Could not connect to network");
  }
}
