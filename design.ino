#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ_PIN 34

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 is common I2C address

const char* ssid = "Dilee";
const char* password = "12345678";

String firebaseURL = "https://ai-agent-rooms-default-rtdb.firebaseio.com/battery_monitor/live.json";

float previousTemp = 0;
String lastStatus = "SAFE";

void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.init();
  lcd.backlight();

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  lcd.setCursor(0,0);
  lcd.print("WiFi Connected");
  delay(2000);
  lcd.clear();
}

void loop() {

  float temp = dht.readTemperature();
  int gasRaw = analogRead(MQ_PIN);

  if (isnan(temp)) {
    Serial.println("DHT Error");
    return;
  }

  float tempRise = temp - previousTemp;
  previousTemp = temp;

  int gasScore = map(gasRaw, 0, 4095, 0, 100);
  float risk = (tempRise * 40) + (gasScore * 0.6);

  String status = "SAFE";
  if (risk > 60) status = "CRITICAL";
  else if (risk > 30) status = "WARNING";

  // Serial Output
  Serial.println("-----");
  Serial.println("Temp: " + String(temp));
  Serial.println("Gas: " + String(gasRaw));
  Serial.println("Risk: " + String(risk));
  Serial.println("Status: " + status);

  // LCD Display
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("C ");

  lcd.print("R:");
  lcd.print((int)risk);

  lcd.setCursor(0,1);
  lcd.print("Gas:");
  lcd.print(gasScore);
  lcd.print("% ");

  lcd.print(status);

  // Firebase Upload
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(firebaseURL);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{";
    jsonData += "\"temperature\":" + String(temp) + ",";
    jsonData += "\"gas\":" + String(gasRaw) + ",";
    jsonData += "\"risk\":" + String(risk) + ",";
    jsonData += "\"status\":\"" + status + "\"";
    jsonData += "}";

    int httpResponseCode = http.PUT(jsonData);
    Serial.println("HTTP: " + String(httpResponseCode));

    http.end();
  }

  delay(3000);
}
