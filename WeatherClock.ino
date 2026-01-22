#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// --- SETTINGS ---
const char* ssid = "SSID";
const char* password = "WIFI PASSWORD";
String apiKey = "API KEY";
String city = "DIST";
String countryCode = "COUNTRY";

int timezone = 5.5 * 3600;
int dst = 0;

#define TFT_CS    D8
#define TFT_RST   D4
#define TFT_DC    D3

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

unsigned long lastWeatherUpdate = 0;
const unsigned long weatherInterval = 600000; 
int currentTemp = 0;
int currentHum = 0;

void setup() {
  tft.init(240, 320); 
  tft.setRotation(1); 
  
  // If your background is White when it should be Black, 
  // uncomment the line below to flip the colors:
  // tft.invertDisplay(true); 

  tft.fillScreen(ST77XX_WHITE); // Setting background to White as requested

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  configTime(timezone, dst, "pool.ntp.org");
  tft.fillScreen(ST77XX_WHITE); 
}

void loop() {
  if (millis() - lastWeatherUpdate > weatherInterval || lastWeatherUpdate == 0) {
    getWeather();
  }
  displayUI();
  delay(1000);
}

void getWeather() {
  WiFiClient client;
  HTTPClient http;
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&appid=" + apiKey + "&units=metric";
  
  if (http.begin(client, url)) {
    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      JsonDocument doc; 
      deserializeJson(doc, payload);
      currentTemp = doc["main"]["temp"].as<int>();
      currentHum = doc["main"]["humidity"].as<int>();
    }
    http.end();
  }
  lastWeatherUpdate = millis();
}

void displayUI() {
  time_t now = time(nullptr);
  struct tm* ptm = localtime(&now);

// 1. HEADING
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE); 
  tft.setCursor(45, 10); 
  tft.print("ANY TEXT HERE");

  // --- TIME SECTION ---
  tft.setCursor(15, 60);
  // Setting text to BLACK, background to WHITE
  tft.setTextColor(ST77XX_BLUE, ST77XX_WHITE); 
  tft.setTextSize(5); 
  
  char timeBuf[10];
  sprintf(timeBuf, "%02d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
  tft.print(timeBuf);

  // --- DIVIDER LINE ---
  tft.drawFastHLine(10, 130, 300, ST77XX_BLACK);

  // --- WEATHER SECTION ---
  tft.setTextSize(3);
  
  // Temp (Keeping it Cyan for visibility, or change to Black if preferred)
  tft.setCursor(40, 160);
  tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
  tft.printf("TEMP: %d C", currentTemp);

  // Humidity (Setting to BLUE)
  tft.setCursor(40, 200);
  tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
  tft.printf("HUMI: %d %%", currentHum);
}
