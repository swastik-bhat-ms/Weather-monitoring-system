# Weather-monitoring-system
🌦️ Weather Monitoring System
This project is a Weather Monitoring System built using the ESP32 microcontroller. It collects real-time environmental data such as temperature, humidity, UV index, light intensity, and atmospheric pressure using various sensors and sends the data to Google Sheets and Blynk IoT for monitoring and analysis.

📌 Features
📡 Real-time data collection from environment
☁️ Temperature and Humidity monitoring (DHT22)
🌤️ Light Intensity measurement (LDR)
🌬️ Atmospheric Pressure measurement (BMP180)
📊 Data logging to Google Sheets
📱 Live monitoring via Blynk IoT app

🔧 Technologies Used
ESP32
DHT22 Sensor
BMP180 Pressure Sensor
LDR (Light Dependent Resistor)
Google Sheets API
Blynk IoT Platform
Arduino IDE

⚙️ How It Works
Sensors connected to ESP32 collect weather data.
The ESP32 processes and formats the data.
Data is sent to:
Google Sheets via Google Apps Script Webhook for storage.
Blynk IoT for real-time visualization on mobile.

🚀 Getting Started
Requirements
ESP32 board
Arduino IDE installed
Internet connection
Google account (for Sheets)
Blynk account and mobile app

Setup Instructions
Clone this repository and open the code in Arduino IDE.
Install required libraries (e.g., Blynk, Adafruit BMP180, DHT).
Enter your Wi-Fi credentials, Blynk Auth Token, and Google Sheets webhook URL in the code.
Upload the code to your ESP32.
Open the Blynk app and monitor your sensor data live.


📬 Contact
📧 [www.swastikbhatms21@gmail.com]

