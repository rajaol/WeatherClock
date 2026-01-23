#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// --- SETTINGS ---
const char* ssid = "wifi";         // Change to your WiFi SSID
const char* password = "wifi";     // Change to your WiFi Password
String apiKey = "api";             // Your OpenWeatherMap API Key
String city = "DT";                // Your City
String countryCode = "COUNTRY";    // Your Country Code

int timezone = 5.5 * 3600;         // UTC offset (5.5 for IST)
int dst = 0;

#define TFT_CS    D8
#define TFT_RST   D4
#define TFT_DC    D3

// Initialize Display (240x320)
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

unsigned long lastWeatherUpdate = 0;
const unsigned long weatherInterval = 600000; // 10 minutes
int currentTemp = 0;
int currentHum = 0;

void setup() {
  tft.init(240, 320); 
  tft.setRotation(1); // Landscape mode
  
  tft.fillScreen(ST77XX_WHITE); 

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Sync time from NTP server
  configTime(timezone, dst, "pool.ntp.org");
  tft.fillScreen(ST77XX_WHITE); 
}

void loop() {
  // Update weather every 10 mins
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

// Helper function to print centered text and clear old characters
void printCentered(const char* buf, int y, int textSize, uint16_t color) {
  int16_t x1, y1;
  uint16_t w, h;
  tft.setTextSize(textSize);
  
  // Calculate width of the text string to center it
  tft.getTextBounds(buf, 0, y, &x1, &y1, &w, &h);
  int x = (320 - w) / 2;
  
  // Set cursor and print with background color to overwrite old text
  tft.setCursor(x, y);
  tft.setTextColor(color, ST77XX_WHITE);
  tft.print(buf);
  
  // Clear a small area after the text just in case the previous string was longer
  tft.print("    "); 
}

void displayUI() {
  time_t now = time(nullptr);
  struct tm* ptm = localtime(&now);

  // 1. HEADING
  printCentered("VU3TBU MK81AE", 10, 3, ST77XX_BLACK);

  // --- TIME SECTION ---
  char timeBuf[10];
  sprintf(timeBuf, "%02d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
  printCentered(timeBuf, 50, 5, ST77XX_BLUE);

  // --- DATE SECTION ---
  char dateBuf[20];
  strftime(dateBuf, sizeof(dateBuf), "%d - %m - %Y", ptm);
  printCentered(dateBuf, 100, 2, ST77XX_BLACK);

  // --- DAY SECTION ---
  // This is where "Fridayday" was happening. printCentered() fixes it.
  char dayBuf[20];
  strftime(dayBuf, sizeof(dayBuf), "%A", ptm);
  printCentered(dayBuf, 120, 2, ST77XX_BLACK);

  // --- DIVIDER LINE ---
  tft.drawFastHLine(20, 145, 280, ST77XX_BLACK);

  // --- WEATHER SECTION ---
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
  
  // Temperature
  tft.setCursor(45, 170);
  tft.printf("TEMP: %d ", currentTemp);
  tft.drawCircle(195, 172, 3, ST77XX_BLACK); // Degree symbol
  tft.setCursor(205, 170);
  tft.print("C  ");

  // Humidity
  tft.setCursor(45, 210);
  tft.printf("HUMI: %d %%  ", currentHum);
}
