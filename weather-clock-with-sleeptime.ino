#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// --- SETTINGS ---
const char* ssid = "WIFI";
const char* password = "PASS";
String apiKey = "KEY";
String city = "DISTIRCT/PLACE";
String countryCode = "COUNTRY";

int timezone = 5.5 * 3600; 
int dst = 0;

#define TFT_CS    D8
#define TFT_RST   D4
#define TFT_DC    D3
#define TFT_BL    D1  // Backlight control pin

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

unsigned long lastWeatherUpdate = 0;
const unsigned long weatherInterval = 600000; 
int currentTemp = 0;
int currentHum = 0;
bool screenIsOn = true; 

void setup() {
  // Initialize Backlight Pin
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // Turn on backlight initially

  tft.init(240, 320); 
  tft.setRotation(1); 
  tft.invertDisplay(false); 
  tft.fillScreen(ST77XX_BLACK); 

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  configTime(timezone, dst, "pool.ntp.org");
}

void loop() {
  time_t now = time(nullptr);
  struct tm* ptm = localtime(&now);
  int currentHour = ptm->tm_hour;

  // OFF during 11 PM (23) to 5 AM (05)
  bool shouldBeOff = (currentHour >= 23 || currentHour < 5);

  if (!shouldBeOff) {
    // --- WAKE UP LOGIC ---
    if (!screenIsOn) {
      digitalWrite(TFT_BL, HIGH);   // Power on the backlight LEDs
      tft.fillScreen(ST77XX_BLACK); // Clear any artifacts
      screenIsOn = true;
    }

    if (millis() - lastWeatherUpdate > weatherInterval || lastWeatherUpdate == 0) {
      getWeather();
    }
    
    displayUI();
  } 
  else {
    // --- SLEEP LOGIC ---
    if (screenIsOn) {
      tft.fillScreen(ST77XX_BLACK); // Clear data from memory
      digitalWrite(TFT_BL, LOW);    // Kill the backlight power completely
      screenIsOn = false;
    }
  }

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

void printCentered(const char* buf, int y, int textSize, uint16_t color) {
  int16_t x1, y1;
  uint16_t w, h;
  tft.setTextSize(textSize);
  tft.getTextBounds(buf, 0, y, &x1, &y1, &w, &h);
  int x = (320 - w) / 2;
  
  tft.setCursor(x, y);
  tft.setTextColor(color, ST77XX_BLACK); 
  tft.print(buf);
  tft.print("    "); 
}

void displayUI() {
  time_t now = time(nullptr);
  struct tm* ptm = localtime(&now);

  printCentered("VU3TBU MK81AE", 10, 3, ST77XX_WHITE);

  char timeBuf[10];
  sprintf(timeBuf, "%02d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
  printCentered(timeBuf, 50, 5, ST77XX_YELLOW);

  char dateBuf[20];
  strftime(dateBuf, sizeof(dateBuf), "%d - %m - %Y", ptm);
  printCentered(dateBuf, 100, 2, ST77XX_WHITE);

  char dayBuf[20];
  strftime(dayBuf, sizeof(dayBuf), "%A", ptm);
  printCentered(dayBuf, 125, 2, ST77XX_CYAN);

  tft.drawFastHLine(20, 155, 280, ST77XX_WHITE);

  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(3);
  tft.setCursor(45, 180);
  tft.printf("TEMP: %d", currentTemp);
  tft.drawCircle(210, 182, 3, ST77XX_WHITE); 
  tft.setCursor(220, 180);
  tft.print("C");

  tft.setCursor(45, 220);
  tft.printf("HUMI: %d %%", currentHum);
}
