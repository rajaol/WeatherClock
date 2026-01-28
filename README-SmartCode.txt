ESP8266 Weather Station & Smart Clock
A sleek, IoT-enabled digital clock and weather display using an ESP8266 (NodeMCU) and an ST7789 240x320 TFT LCD. This project fetches real-time weather data from OpenWeatherMap and synchronizes time via NTP (Network Time Protocol).
🌟 Features
•	Real-time Clock: Synchronizes with pool.ntp.org for high-accuracy timekeeping.
•	Live Weather: Displays current Temperature ($^\circ C$) and Humidity (%) using the OpenWeatherMap API.
•	Automatic Sleep Mode: Saves power and prevents light pollution by turning off the display and hardware backlight between 11:00 PM and 5:00 AM.
•	Backlight Control: Uses a dedicated GPIO pin to physically cut power to the LCD backlight for a "True Black" sleep state.
•	Optimized UI: High-contrast color scheme (Yellow/Cyan on Black) designed for the ST7789 IPS display.
🛠️ Hardware Requirements
•	Microcontroller: ESP8266 (NodeMCU or Wemos D1 Mini).
•	Display: ST7789 TFT LCD (240x320).
•	Connection Wires: Jumper wires.
Wiring Diagram
ST7789 Pin	ESP8266 Pin	Description
VCC	3V3	Power
GND	GND	Ground
SCL	D5 (GPIO14)	SPI Clock
SDA	D7 (GPIO13)	SPI Data
RES	D4 (GPIO2)	Reset
DC	D3 (GPIO0)	Data/Command
CS	D8 (GPIO15)	Chip Select
BLK / LED	D1 (GPIO5)	Backlight Control
🚀 Setup & Installation
1.	Libraries: Install the following via the Arduino Library Manager:
o	Adafruit_GFX
o	Adafruit_ST7789
o	ArduinoJson
2.	API Key: Sign up at OpenWeatherMap and get a free API key.
3.	Configuration: Update the following variables in the code:
C++
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
String apiKey = "YOUR_OPENWEATHER_API_KEY";
4.	Upload: Select your ESP8266 board in the Arduino IDE and hit upload.
🌙 Smart Power Logic
The display utilizes a state-machine logic to manage the screen:
•	Active (05:00 - 22:59): The display is active, updating the time every second and weather every 10 minutes.
•	Sleep (23:00 - 04:59): The code calls digitalWrite(TFT_BL, LOW) to kill the backlight and clears the frame buffer to reduce power consumption and heat.
📜 License
This project is open-source. Feel free to modify the UI or time-offsets to suit your location.

