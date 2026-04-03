# ⚡ EV Battery Monitor — Real-Time Heat & Gas Detection System

![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![Sensors](https://img.shields.io/badge/Sensors-DHT11%20%7C%20MQ2-green)
![Backend](https://img.shields.io/badge/Backend-Firebase%20Realtime%20DB-orange)
![Frontend](https://img.shields.io/badge/Frontend-HTML%20%2F%20JS-yellow)
![Status](https://img.shields.io/badge/Status-Active-brightgreen)

> A real-time IoT monitoring system for EV battery safety — detects abnormal heat and hazardous gas levels, calculates a live risk score, displays alerts on an LCD, and streams data to Firebase for a live web dashboard.

---

## 🔍 Problem Statement

Electric Vehicle batteries are prone to **thermal runaway** — a chain reaction where overheating leads to fire or explosion. Early detection of rising temperature and gas buildup (hydrogen, CO) is critical for battery safety. This project implements a low-cost, real-time monitoring solution using ESP32 and cloud infrastructure.

---

## 🏗️ System Architecture

```
[DHT11 Temp Sensor] ──┐
                        ├──► [ESP32] ──WiFi──► [Firebase Realtime DB] ──► [Web Dashboard]
[MQ Gas Sensor]    ────┤
                        │
[I2C LCD Display]  ◄───┘  (local real-time display)
```

---

## ⚙️ Hardware Components

| Component | Model | Purpose |
|-----------|-------|---------|
| Microcontroller | ESP32 | WiFi connectivity + sensor processing + risk calculation |
| Temperature Sensor | DHT11 | Detect battery heat levels and rate of rise |
| Gas Sensor | MQ-2 / MQ-135 | Detect hazardous gas leakage (H₂, CO, LPG) |
| LCD Display | 16x2 I2C (0x27) | Local real-time display of temp, gas, risk and status |
| Power Supply | 3.3V / 5V | ESP32 + sensor power |

---

## 🛠️ Tech Stack

**Firmware**
- ESP32 Arduino SDK (C++)
- WiFi + HTTPClient library for wireless data transfer
- Firebase REST API (HTTP PUT) for real-time sync
- DHT library (Adafruit) for temperature readings
- LiquidCrystal_I2C for LCD display

**Backend**
- Firebase Realtime Database
- Live data streaming via REST API (HTTP PUT)

**Frontend**
- HTML / CSS / JavaScript
- Firebase JS SDK for real-time updates
- Live chart and alert dashboard

---

## 🧠 Risk Score Algorithm

```cpp
float tempRise = temp - previousTemp;           
int gasScore   = map(gasRaw, 0, 4095, 0, 100);  

float risk = (tempRise * 40) + (gasScore * 0.6);
```

| Factor | Weight | Reason |
|--------|--------|--------|
| Temperature rise rate | × 40 | Sudden spikes are more dangerous than absolute value |
| Gas concentration | × 0.6 | Sustained gas presence adds to overall risk |

**Why rate of change matters:**  
Real Battery Management Systems (BMS) monitor the *rate* of temperature increase, not just the absolute temperature. A battery at 60°C that rose 2°C in 3 seconds is far more dangerous than one stable at 60°C for hours.

### Alert Thresholds

| Risk Score | Status | Action |
|------------|--------|--------|
| 0 – 30 | ✅ SAFE | Normal operation |
| 31 – 60 | ⚠️ WARNING | Monitor closely |
| > 60 | 🔴 CRITICAL | Immediate alert |

---

## 🚀 Features

- 🌡️ Real-time temperature monitoring (every 3 seconds)
- 💨 Gas level detection (0–100% normalized)
- 📈 Dynamic risk scoring algorithm
- 🖥️ LCD real-time display
- ☁️ Firebase cloud sync
- 📊 Live web dashboard
- 📱 Remote monitoring via browser

---

## 📁 Project Structure

```
ev-battery-monitor/
│
├── firmware/
│   └── ev_battery_monitor.ino
│
├── frontend/
│   ├── index.html
│   ├── style.css
│   └── app.js
│
└── README.md
```

---

## 🔧 Setup & Installation

### 1. Firebase Setup
1. Go to https://console.firebase.google.com/
2. Create a new project → Enable **Realtime Database**
3. Set rules:
```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```
4. Copy config credentials

---

### 2. ESP32 Firmware
1. Open `.ino` file in Arduino IDE  
2. Install:
   - DHT sensor library (Adafruit)
   - LiquidCrystal I2C  

3. Update:
```cpp
const char* ssid     = "your_wifi_name";
const char* password = "your_wifi_password";
String firebaseURL   = "https://your-project.firebaseio.com/battery_monitor/live.json";
```

4. Upload to ESP32

---

### 3. Frontend
1. Update Firebase config in `app.js`
2. Open `index.html` → Dashboard runs live ✅

---

## 📊 Firebase Data Structure

```json
{
  "battery_monitor": {
    "live": {
      "temperature": 42.5,
      "gas": 1850,
      "risk": 34.6,
      "status": "WARNING"
    }
  }
}
```

---

## 🎯 Real-World Applications

- EV battery packs
- Li-ion storage rooms
- EV charging stations
- Industrial backup systems

---

## 🔮 Future Improvements

- SMS / Email alerts (Twilio / EmailJS)
- Historical data analytics
- Flutter mobile app
- ML-based anomaly detection
- CAN bus / OBD-II integration
- Upgrade to DHT22 sensor

---

## 👨‍💻 Author

**Dileep Kumar M**  
BE ECE | Cambridge Institute of Technology  
🔗 https://www.linkedin.com/in/dileep-kumarm  
💻 https://github.com/dileepkumar33  

---

## 📄 License

This project is open source under the MIT License.
