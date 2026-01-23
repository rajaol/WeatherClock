# 🌐 ESP8266 WiFi Clock with Weather (ST7789 TFT)

A simple yet elegant **WiFi-enabled digital clock with live weather display**, built using **ESP8266** and a **240×320 ST7789 TFT display**. The project synchronizes time via **NTP**, fetches real-time weather data from **OpenWeatherMap**, and presents everything on a clean **white-background UI**.

---

## ✨ Features

* ⏰ **Accurate Time via NTP** (auto-synced from the internet)
* 🌡️ **Live Temperature Display** (°C)
* 💧 **Live Humidity Display** (%)
* 🌍 Weather data from **OpenWeatherMap API**
* 📶 WiFi connectivity using ESP8266
* 🎨 Clean, readable **ST7789 TFT UI**
* 🕒 Timezone configurable (default: IST)
* 🔄 Weather auto-refresh every **10 minutes**

---

## 🧰 Hardware Required

* ESP8266 (NodeMCU / ESP-12 / ESP8266 Dev Board)
* 1.3" or 1.54" **ST7789 TFT Display (240×320)**
* WiFi Internet Connection
* Jumper wires
* USB cable

---

## 🔌 Wiring Connections

| ST7789 Pin | ESP8266 Pin |
| ---------- | ----------- |
| CS         | D8          |
| DC         | D3          |
| RST        | D4          |
| SCK        | D5 (SCLK)   |
| MOSI       | D7 (MOSI)   |
| VCC        | 3.3V        |
| GND        | GND         |

> ⚠️ **Note:** Use **3.3V only** for the display to avoid damage.

---

## 📦 Required Libraries

Install the following libraries via **Arduino Library Manager**:

* `ESP8266WiFi`
* `ESP8266HTTPClient`
* `ArduinoJson`
* `Adafruit GFX Library`
* `Adafruit ST7789`

---

## 🌐 API & Configuration

### OpenWeatherMap

1. Create a free account at: [https://openweathermap.org/](https://openweathermap.org/)
2. Generate your **API Key**

### Update These in Code

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
String apiKey = "YOUR_OPENWEATHER_API_KEY";
String city = "YOUR_CITY";
String countryCode = "YOUR_COUNTRY_CODE";
```

### Timezone Settings

```cpp
int timezone = 5.5 * 3600;  // IST
int dst = 0;
```

---

## 🖥️ Display Layout

* **Top:** Custom heading text
* **Center:** Large digital clock (HH:MM:SS)
* **Divider line**
* **Bottom:**

  * Temperature (°C)
  * Humidity (%)

White background ensures excellent readability in indoor environments.

---

## 🔄 How It Works

1. ESP8266 connects to WiFi
2. Time is synchronized via **NTP server**
3. Weather data is fetched from **OpenWeatherMap API**
4. Data is parsed using **ArduinoJson**
5. UI is refreshed every second
6. Weather updates every 10 minutes

---

## 🧪 Tested On

* ESP8266 NodeMCU
* ST7789 240×320 TFT
* Arduino IDE 2.x

---

## 🚀 Future Enhancements

* 🌦️ Weather icons
* 📊 Forecast view
* 🔆 Auto brightness control
* 📡 Offline RTC fallback
* 🌙 Dark mode toggle

---

## 📜 License

This project is open-source and free to use for **personal and educational purposes**.

---

## 🙌 Credits

* OpenWeatherMap API
* Adafruit Graphics Libraries
* ESP8266 Community

---

**73 & Happy Hacking!** 🛠️📡


------------------------------------part 2 ---------------------------------------
ESP8266 Weather & Station Clock 🌡️🕒A sleek, real-time dashboard powered by an ESP8266 and an ST7789 TFT display. This project synchronizes with NTP servers for high-precision time and fetches live local weather data using the OpenWeatherMap API.🌟 FeaturesLive Weather: Real-time Temperature and Humidity updates via OpenWeatherMap.Precision Time: Synchronized with pool.ntp.org via WiFi.Auto-Formatting: Displays Date in DD - MM - YYYY format and full Day of the Week.Clean UI: High-contrast Black/Blue text on a White background with auto-centering logic to prevent text ghosting.Hardware Optimized: Specifically tuned for the ST7789 240x320 IPS display.🛠️ Hardware RequiredMicrocontroller: ESP8266 (NodeMCU or Wemos D1 Mini).Display: ST7789 240x320 TFT (SPI Interface).Connecting Wires: Jumper wires.Pin MappingST7789 PinESP8266 PinNoteVCC3V3PowerGNDGNDGroundSCLD5 (GPIO14)SPI ClockSDAD7 (GPIO13)SPI MOSIRESD4 (GPIO2)ResetDCD3 (GPIO0)Data/CommandCSD8 (GPIO15)Chip Select🚀 Installation & SetupLibraries: Install the following libraries via the Arduino Library Manager:Adafruit GFX LibraryAdafruit ST7789 LibraryArduinoJson (v6 or higher)API Key:Sign up at OpenWeatherMap.Generate a free API key and paste it into the apiKey variable in the code.Configuration:Update the following variables in the .ino file:C++const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
String apiKey = "YOUR_API_KEY";
String city = "YOUR_CITY";
String countryCode = "YOUR_COUNTRY_CODE";
Upload: Select your ESP8266 board in the Arduino IDE and click Upload.🖥️ User Interface LogicThe code utilizes a custom printCentered() function. This solves the common "text ghosting" issue where a shorter word (like "Friday") leaves behind letters from a previously displayed longer word (like "Wednesday"). It calculates the string width dynamically and clears the background for a crisp look.📄 LicenseThis project is open-source. Feel free to fork and modify!
