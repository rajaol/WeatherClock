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
const char* password = "PASS";
String apiKey = "API";
String LAT = "11.6657";
String LON = "76.2163";

int timezone = 5.5 * 3600; 
int dst = 0;

#define TFT_CS    D8
#define TFT_RST   D4
#define TFT_DC    D3
#define TFT_BL    D1  

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

unsigned long lastWeatherUpdate = 0;
const unsigned long weatherInterval = 600000; 
unsigned long lastClockUpdate = 0;
int lastMin = -1; 

int currentTemp = 0;
int currentHum = 0;

void setup() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); 

  // Initialize display with 40MHz SPI speed for smooth redraws
  tft.init(240, 320, SPI_MODE0); 
  SPI.setFrequency(40000000); 
  
  tft.setRotation(1); 
  tft.invertDisplay(false); 
  tft.fillScreen(ST77XX_BLACK); 

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  configTime(timezone, dst, "pool.ntp.org");
}

void loop() {
  unsigned long currentMillis = millis();
  time_t now = time(nullptr);
  struct tm* ptm = localtime(&now);
  
  if (ptm->tm_year < 70) return; 

  // Weather Update Logic
  if (currentMillis - lastWeatherUpdate >= weatherInterval || lastWeatherUpdate == 0) {
    getWeather(); 
    lastWeatherUpdate = currentMillis;
  }

  // Clock and UI Update Logic
  if (currentMillis - lastClockUpdate >= 1000) {
    drawSeconds(ptm->tm_sec);
    lastClockUpdate = currentMillis;
    
    // Staggered Redraw: Update heavy UI at 1s past the minute
    if (ptm->tm_min != lastMin && ptm->tm_sec == 1) {
      displayStaticUI(ptm);
      lastMin = ptm->tm_min;
    }
  }
}

void drawSeconds(int sec) {
  tft.setTextSize(4);
  tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
  tft.setCursor(245, 60); 
  
  char secBuf[5];
  sprintf(secBuf, "%02d", sec);
  tft.print(secBuf);
}

void displayStaticUI(struct tm* ptm) {
  // 1. Heading
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setCursor(45, 10);
  tft.print("VU2MIE MK81CP");

  // 2. HH:MM (Yellow)
  char hhmm[10];
  sprintf(hhmm, "%02d:%02d", ptm->tm_hour, ptm->tm_min);
  tft.setTextSize(7); 
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
  tft.setCursor(15, 45); 
  tft.print(hhmm);

  // 3. Date
  char dateBuf[20];
  strftime(dateBuf, sizeof(dateBuf), "%d - %m - %Y", ptm);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setCursor(75, 105);
  tft.print(dateBuf);

  // 4. DAY (Capitalized and Increased Font Size)
  char dayBuf[20];
  strftime(dayBuf, sizeof(dayBuf), "%A", ptm);
  String dayStr = String(dayBuf);
  dayStr.toUpperCase(); 
  
  tft.setTextSize(3); 
  tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
  
  int dayX = (320 - (dayStr.length() * 18)) / 2; 
  tft.setCursor(dayX, 128); 
  tft.print(dayStr);
  tft.print("   "); 

  tft.drawFastHLine(20, 158, 280, ST77XX_WHITE);

  // 5. Weather Section
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  
  tft.setCursor(45, 172);
  tft.printf("TEMP: %d", currentTemp);
  tft.drawCircle(210, 174, 3, ST77XX_WHITE); 
  tft.setCursor(220, 172);
  tft.print("C");

  tft.setCursor(45, 208);
  tft.printf("HUMI: %d %%", currentHum);
}

void getWeather() {
  WiFiClient client;
  HTTPClient http;
  String url = "http://api.openweathermap.org/data/2.5/weather?lat=" + LAT + "&lon=" + LON + "&appid=" + apiKey + "&units=metric";
  http.setTimeout(1000); 
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
}
