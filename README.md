# 🚨 Fall Detection System
### ESP12-E + MPU6050 + Edge Impulse AI + Telegram Alert

![Platform](https://img.shields.io/badge/Platform-ESP8266%20NodeMCU-blue)
![AI](https://img.shields.io/badge/AI-Edge%20Impulse-green)
![Sensor](https://img.shields.io/badge/Sensor-MPU6050-orange)
![Accuracy](https://img.shields.io/badge/Accuracy-96.3%25-brightgreen)

> ⚡ Real-time fall detection using on-device AI inference — alerts are sent instantly via Telegram bot when a fall is detected.

---

## 📌 Table of Contents

- [Overview](#-overview)
- [Hardware Components](#-hardware-components)
- [Pin Connections](#-pin-connections)
- [System Architecture](#-system-architecture)
- [Edge Impulse Setup](#-edge-impulse-setup)
- [Model Performance](#-model-performance)
- [Libraries Required](#-libraries-required)
- [Arduino Board Settings](#-arduino-board-settings)
- [Project File Structure](#-project-file-structure)
- [Configuration](#-configuration)
- [Telegram Bot Setup](#-telegram-bot-setup)
- [How It Works](#-how-it-works)
- [Common Errors & Fixes](#-common-errors--fixes)
- [Author](#-author)

---

## 🧠 Overview

This project is an **IoT-based Fall Detection System** that uses a **MPU6050 accelerometer** sensor connected to an **ESP12-E (NodeMCU)** microcontroller. An AI model trained on **Edge Impulse** runs directly on the device to classify motion as either `fall` or `normal`.

When a fall is detected with confidence > **90%**, the system:
- 💡 Flashes the **LED**
- 🔔 Triggers the **Buzzer**
- 📲 Sends a **Telegram alert message** immediately

---

## 🔧 Hardware Components

| Component | Details | Purpose |
|-----------|---------|---------|
| **ESP12-E (NodeMCU)** | ESP8266 based WiFi Module | Main microcontroller |
| **MPU6050** | 6-axis IMU Sensor | Accelerometer data collection |
| **LED** | Built-in GPIO2 (D4) | Visual fall alert indicator |
| **Buzzer** | GPIO14 (D5) | Sound alert on fall detection |
| **Power Supply** | 3.3V / 5V USB | Board power |

---

## 🔌 Pin Connections

### MPU6050 → ESP12-E

| MPU6050 Pin | ESP12-E Pin | GPIO | Note |
|-------------|-------------|------|------|
| VCC | 3.3V | — | Do NOT connect to 5V |
| GND | GND | — | Common ground |
| SDA | D2 | GPIO4 | I2C Data line |
| SCL | D1 | GPIO5 | I2C Clock line |
| AD0 | GND | — | I2C address = 0x68 |
| INT | Not connected | — | Not used |

### LED & Buzzer

| Component | ESP12-E Pin | GPIO | Note |
|-----------|-------------|------|------|
| Built-in LED | D4 | GPIO2 | `LOW` = ON, `HIGH` = OFF |
| Buzzer (+) | D5 | GPIO14 | `HIGH` = ON, `LOW` = OFF |
| Buzzer (−) | GND | — | — |

---

## 🏗️ System Architecture

```
┌─────────────┐     I2C      ┌────────────────────────────────────┐
│  MPU6050    │─────────────▶│         ESP12-E (NodeMCU)          │
│ (ax, ay, az)│              │                                    │
└─────────────┘              │  1. Collect 2s accelerometer data  │
                             │  2. Run Edge Impulse AI Model      │
                             │  3. Classify: FALL / NORMAL        │
                             │                                    │
                             │  If Fall > 0.90 confidence:        │
                             │  ├── Flash LED (GPIO2)             │
                             │  ├── Buzz Buzzer (GPIO14)          │
                             │  └── Send Telegram Alert           │
                             └──────────────┬─────────────────────┘
                                            │  WiFi (HTTPS)
                                            ▼
                                   ┌─────────────────┐
                                   │  Telegram Bot   │
                                   │  @RSMAURYA_bot  │
                                   └─────────────────┘
```

---

## ⚙️ Edge Impulse Setup

### Step 1 — Data Collection Settings

| Setting | Value |
|---------|-------|
| Sample Length | 5000 ms (5 seconds) |
| Frequency | 100 Hz |
| Sensor | 3-axis Accelerometer (ax, ay, az) |
| Labels | `fall`, `normal` |
| Samples per label | 20–30 minimum |

### Step 2 — Create Impulse Settings

| Setting | Value |
|---------|-------|
| Window Size | 2000 ms |
| Window Stride | 200 ms |
| Frequency | 100 Hz |
| Processing Block | Spectral Analysis |
| Learning Block | Classification (Keras) |

### Step 3 — Spectral Analysis Settings

| Setting | Value |
|---------|-------|
| Scale axes | 1 |
| Input decimation ratio | 1 |
| Filter Type | None |
| Analysis Type | FFT |
| FFT length | 16 |
| Take log of spectrum | ✅ Yes |
| Overlap FFT frames | ✅ Yes |

### Step 4 — Deployment Settings

| Setting | Value |
|---------|-------|
| Deployment Target | Arduino Library |
| Inference Engine | TensorFlow Lite (**EON OFF**) |
| Model Optimization | Quantized (int8) |

---

## 📊 Model Performance

| Metric | Value |
|--------|-------|
| **Accuracy** | **96.3%** |
| Loss | 0.12 |
| Fall Detection Rate | 93.1% |
| Normal Detection Rate | 100% |
| F1 Score | 0.96 |
| Area under ROC Curve | 0.97 |

---

## 📦 Libraries Required

Install these in Arduino IDE:

```
1. Wire.h                    — Built-in (I2C communication)
2. MPU6050                   — by Electronic Cats (Library Manager)
3. falldetection_inferencing — Edge Impulse exported ZIP (add manually)
4. ESP8266WiFi               — Built-in with ESP8266 board package
5. ESP8266HTTPClient         — Built-in with ESP8266 board package
6. WiFiClientSecureBearSSL   — Built-in with ESP8266 board package
```

> 💡 To install Edge Impulse library: `Sketch → Include Library → Add .ZIP Library` → select `ei-falldetection-arduino-1.0.2.zip`

---

## 🛠️ Arduino Board Settings

| Setting | Value |
|---------|-------|
| **Board** | NodeMCU 1.0 (ESP-12E Module) |
| Upload Speed | 115200 |
| Flash Size | 4MB |
| CPU Frequency | 80 MHz (default) |

---

## 📁 Project File Structure

```
project 3 fall detection/
│
├── 📄 FallDetection_Notes.docx          ← Complete project notes & documentation
│
├── 📁 falldetectioncode1/
│   └── falldetectioncode1/
│       └── falldetectioncode1.ino       ← ✅ MAIN CODE — Fall detection + Telegram alert
│
├── 📁 fall_data_callection_code/
│   └── fall_data_callection_code.ino    ← Data collection code for Edge Impulse training
│
├── 📁 laibrary/
│   ├── ei-falldetection-arduino-1.0.2.zip  ← Edge Impulse trained model library
│   ├── falldetection.zip                   ← Alternate model ZIP
│   ├── ESP8266HttpClient-master.zip        ← HTTP client library backup
│   └── ESP8266HTTPClient.h                 ← HTTP client header
│
└── 🖼️ WhatsApp Image 2026-02-22.jpeg   ← Hardware setup photo
```

---

## 🔐 Configuration

Before uploading `falldetectioncode1.ino`, update these values:

```cpp
// WiFi Credentials
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Telegram Bot
const String token   = "YOUR_BOT_TOKEN";       // From @BotFather
const String chat_id = "YOUR_CHAT_ID";          // From @userinfobot
```

> ⚠️ **Security Warning:** Never share your WiFi password or Telegram Bot token in public repositories.

---

## 📲 Telegram Bot Setup

| Detail | Value |
|--------|-------|
| Bot Name | `RSMAURYA_bot` |
| Bot URL | `t.me/RSMAURYA_bot` |
| Alert Message | `⚠️ FALL DETECTED! Someone has fallen! Please come immediately!` |

**Steps to create your own Telegram bot:**
1. Open Telegram → Search `@BotFather`
2. Send `/newbot` and follow instructions
3. Copy the **API Token**
4. Search `@userinfobot` → Send `/start` → Copy your **Chat ID**
5. Paste both values in the code

---

## 🔄 How It Works

| Step | Process |
|------|---------|
| 1 | MPU6050 collects accelerometer data `(ax, ay, az)` at **100 Hz** |
| 2 | A **2-second window** of data is buffered (200 readings) |
| 3 | **Edge Impulse AI model** performs spectral analysis + classification |
| 4 | Model outputs a confidence score: `0.0` (Normal) → `1.0` (Fall) |
| 5 | If `Fall > 0.90` → **FALL DETECTED** is triggered |
| 6 | **LED flashes** + **Buzzer sounds** + **Telegram message sent** |

---

## 🐛 Common Errors & Fixes

| Error | Cause | Fix |
|-------|-------|-----|
| `rst cause:2` boot loop | Code crash / memory issue | Check `Wire.begin(4, 5)` placement |
| `Sensor FAILED` | MPU6050 not found | Check wiring, add `delay(500)` before `mpu.initialize()` |
| `Fall: 0.00, Normal: 0.00` | Label case mismatch | Use index `[0]` instead of `strcmp` |
| `Telegram Error: -1` | HTTPS connection fail | Use `BearSSL::WiFiClientSecure` with `setInsecure()` |
| `Telegram Error: 301` | HTTP redirect to HTTPS | Use `https://` not `http://` in URL |
| `undefined reference SOFTMAX` | EON Compiler conflict | Deploy with **TensorFlow Lite (EON OFF)** |
| `exit status 1` | Missing `setup()`/`loop()` | `Ctrl+A` → Delete → Paste fresh code |

---

## 👨‍💻 Author

**RSM Maurya**

> 📌 *Fall Detection System using Edge Impulse + ESP12-E (NodeMCU)*
> Built with ❤️ for safety and real-time IoT applications.

---

*Last Updated: February 2026*
