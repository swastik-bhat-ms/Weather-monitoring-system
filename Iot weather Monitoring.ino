#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID ""    // Replace with your Blynk Template ID
#define BLYNK_TEMPLATE_NAME ""  // Replace with your Blynk Template Name

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Arduino.h"
#include "DHT.h"
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <HTTPClient.h>

// Sensor variables
float temperature;
float humidity;
float pressure;
float mbar;
int visible;
float heatIndex;
float due;
float altitude;
double luxv;

// Initialize sensors
Adafruit_BMP085 bmp180;
#define LDR_PIN 13

char auth[] = "";  // Replace with your Blynk Auth Token
char ssid[] = "";  // Replace with your WiFi name
char pass[] = "";  // Replace with your WiFi password
String sheetURL="";//Replace with your Google App script sheet url 

#define DHTPIN 4
#define DHTTYPE DHT22  // Ensure only one DHT type is active

HTTPClient http;      //Http initiasize


DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
// connection for wifi
void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, pass);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {  // 10-second timeout
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Connection Failed! Retrying...");
  }
}

void sendtoSheets(){
 if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();  // Reconnect if WiFi is lost
  }
  //Uploading to Sheets
  if (WiFi.status() == WL_CONNECTED) {
  String url = sheetURL + "?temp=" + String(temperature) +"&hum=" + String(humidity) +"&pressure=" + String(mbar) + "&light=" + String(visible) + "&heat=" + String(heatIndex) + "&alti=" + String(altitude) + "&due=" + String(due) + "&luxv=" + String(luxv);
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == 200) {
          Serial.println("Data Sent Successfully to Google Sheets!");
    } else {
          Serial.println("Failed to send data! HTTP Code: " + String(httpCode)); 
    }
  http.end();
  }
  else{
    Serial.println("WiFi Disconnected! Reconnecting...");
  }
}

//To calculate Heat index
float calculateHeatIndex(float T, float H) {
    return -8.784 + (1.611 * T) + (2.338 * H) - (0.146 * T * H) - (0.0123 * T * T) - (0.0164 * H * H) + (0.0022 * T * T * H) + (0.0007 * T * H * H) - (0.0003 * T * T * H * H);
}

//To Calculate Altitude estimation
float calculateAltitude(float pressure) {
    return (44330 * (1.0 - pow((pressure / 1013.25),0.1903)));
}

// To Cal Due Point
float calculateDewPoint(float T, float H) {
    float a = 17.27;
    float b = 237.7;
    float alpha = ((a * temperature) / (b + temperature)) + log(humidity / 100.0);
    float dewPoint = (b * alpha) / (a - alpha);
    return dewPoint;
}

//Light intensity 
double calculateLux(int ldrValue) {
 return (ldrValue == 1) ? 1.0 : 100.0;  // Adjust based on your calibration
}
// Send to Blynk
void sendSensor()
{
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  visible =digitalRead(LDR_PIN);
  // Serial.println(visible);
  // Serial.println(digitalRead(LDR_PIN));
  // Serial.println(analogRead(LDR_PIN));

  if (isnan(humidity) || isnan(temperature)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
   if (!bmp180.begin()) 
  {
    Serial.println("Failed to read from BMP180 sensor!");
    return;
  }

  pressure = bmp180.readPressure();

  // pressure=0 ;   // just for temprory purpose
  mbar = pressure / 100;

  

  heatIndex=dht.computeHeatIndex(temperature, humidity, false);
  due=calculateDewPoint(temperature,humidity);
  altitude=calculateAltitude(mbar);
  luxv=calculateLux(visible);
  Serial.println("===== Sensor Readings =====");
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Pressure: "); Serial.print(mbar); Serial.println(" mbar");
  Serial.print("Visible Light: 1:Dark 2:Light   =>"); Serial.print(visible); Serial.println(" ");
  Serial.print("heat Index: "); Serial.print(heatIndex); Serial.println(" C");
  Serial.print("altitude: "); Serial.print(altitude); Serial.println(" ");
  Serial.print("Due: "); Serial.print(due); Serial.println(" ");
  Serial.print("Lux Value: "); Serial.print(luxv,3); Serial.println(" Lux");
  Serial.println("===========================");
  Serial.println();
  //sending the data to sheets
  sendtoSheets();
  // sending the data to Blynk
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, mbar);
  Blynk.virtualWrite(V3, visible);
  Blynk.virtualWrite(V4, heatIndex);
  Blynk.virtualWrite(V6, due);
  Blynk.virtualWrite(V5, altitude);
  Blynk.virtualWrite(V7, luxv);
}

void keepBlynkAlive() {
  Blynk.virtualWrite(V10, millis() / 1000);
  Serial.println("Sent an Up signal");
  Serial.println("===========================");// Send uptime to keep connection active
}


void setup()
{
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);

  if (!bmp180.begin()) 
  {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
  }

  dht.begin();
  delay(1000);
  connectWiFi();
  Blynk.config(auth);
  timer.setInterval(30000L, keepBlynkAlive);
  timer.setInterval(20000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run(); 
}
