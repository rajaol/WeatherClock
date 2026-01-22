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
