void wifiConnect(const char* ssid, const char* password) {
  uint8_t count = 20;
  // Connect to wifi
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

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

void printLocalTime() {
  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void getNetworkTime() {
  // copyDownloadImage();
  wifiConnect(ssid, password);

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}
