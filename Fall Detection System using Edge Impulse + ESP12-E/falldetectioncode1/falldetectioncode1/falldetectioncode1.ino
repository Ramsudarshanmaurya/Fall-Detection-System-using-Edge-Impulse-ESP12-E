#define EI_TFLITE_DISABLE_ALL 1  // ← Yeh add karo sabse pehle
#include <Wire.h>
#include <MPU6050.h>
#include <falldetection_inferencing.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

MPU6050 mpu;
float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

#define LED_PIN    
#define BUZZER_PIN 14

const char* ssid     = "RS Maurya";
const char* password = "RS8808044818";

const String token   = "8588271297:AAHGgMaU408z5J_QMBkZV0vLxyiv9g_4FLk";
const String chat_id = "6824035595";

int get_data(size_t offset, size_t len, float *out) {
  memcpy(out, features + offset, len * sizeof(float));
  return 0;
}

void ledAlert() {
  digitalWrite(LED_PIN, LOW);
  delay(1000);
  digitalWrite(LED_PIN, HIGH);
}

void buzzerAlert() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("WiFi connect ho raha hai");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void telegramAlert() {
  if (WiFi.status() == WL_CONNECTED) {
    BearSSL::WiFiClientSecure client;
    client.setInsecure();
    client.setBufferSizes(512, 512); // ← Yeh add karo
    HTTPClient http;
    http.setTimeout(10000);

    String url = "https://api.telegram.org/bot" + token +
             "/sendMessage?chat_id=" + chat_id +
             "&text=⚠️+FALL+DETECTED!+Someone+has+fallen!+Please+come+immediately!";

    http.begin(client, url);
    http.addHeader("Connection", "close"); // ← Yeh add karo
    int code = http.GET();

    Serial.print("Response code: ");
    Serial.println(code);

    if (code == 200) {
      Serial.println("Telegram: Bhej diya!");
    } else {
      Serial.print("Telegram Error: ");
      Serial.println(code);
    }
    http.end();
    client.stop(); // ← Yeh add karo
  } else {
    Serial.println("WiFi nahi hai!");
  }
}

void collectData() {
  for (int i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i += 3) {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    features[i]   = ax / 16384.0;
    features[i+1] = ay / 16384.0;
    features[i+2] = az / 16384.0;
    delay(10);
  }
}

float runModel() {
  signal_t signal;
  signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
  signal.get_data = &get_data;
  ei_impulse_result_t result = {0};
  run_classifier(&signal, &result, false);
  return result.classification[0].value;
}

void fallAlert() {
  Serial.println("FALL DETECTED!");
  ledAlert();
  buzzerAlert();
  telegramAlert();
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5);
  delay(500);
  mpu.initialize();
  delay(200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println(mpu.testConnection() ? "Sensor OK" : "Sensor FAILED");
  
  connectWiFi();
  
  // WiFi details print karo
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal: ");
  Serial.println(WiFi.RSSI());
}

void loop() {
  collectData();
  float fallValue = runModel();
  if (fallValue > 0.90) {
    fallAlert();
  } else {
    Serial.print("Fall : ");
    Serial.println(fallValue, 2);
    Serial.print("Normal: ");
    Serial.println(1.0 - fallValue, 2);
    Serial.println("---");
  }
}