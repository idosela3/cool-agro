#include <Wire.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "ThingSpeak.h"
#include "Adafruit_SHT31.h"

// Initialize OLED Display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Initialize SHT31 Sensor
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// WiFi Credentials
const char* ssid = "agrotech";
const char* password = "1Afuna2gezer";

// ThingSpeak Channel Information
unsigned long channelID = 2799038;
const char* writeAPIKey = "UC49K3PIIR0N7G5E";

// MQTT Broker Information
const char* mqtt_server = "192.168.0.102";
const int mqtt_port = 1883;
const char* mqtt_user = "mqtt-user";
const char* mqtt_password = "1234";
const char* mqtt_topic = "/greenhouse/outside/irrigation/solenoid5";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Pin Definitions
const int buttonPin = A4;       // Button to control OLED
const int rainSensorPin = A0;   // Rain sensor input pin

// Variables for Rain Sensor
const float rainPerClick = (50.0 / 24.0) * (10.0 / 55.0); // mm per click
volatile int rainPulses = 0;   // Rain pulses during data collection
float totalRainfall = 0.0;
bool wasHigh = false;          // Tracks voltage transitions for the rain sensor

// Variables for OLED Display
int buttonPressCount = 0;       // Track button press count
unsigned long lastButtonPress = 0;
const unsigned long displayTimeout = 30000; // OLED timeout (30 seconds)
bool displayActive = false;     // Track if OLED is active

// Variables for Button Handling
int lastButtonState = HIGH;     // Previous state of the button
int currentButtonState = HIGH;  // Current state of the button
bool buttonHandled = false;     // To ensure one press is handled once

// Variables for Data Upload
unsigned long lastUploadTime = 0;
const unsigned long uploadInterval = 60000; // 1 minute

// Function Prototypes
void connectWiFi();
void connectMQTT();
void uploadDataToThingSpeak();
void displayMessage(int index);
void controlIrrigation();
void handleRainSensor();
void enterDeepSleep();

void IRAM_ATTR rainPulseISR() {
  rainPulses++;
}

void setup() {
  Serial.begin(115200);

  // Initialize WiFi
  connectWiFi();

  // Initialize MQTT
  mqttClient.setServer(mqtt_server, mqtt_port);

  // Initialize OLED
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.sendBuffer(); // Start with OLED off

  // Initialize SHT31 Sensor
  if (!sht31.begin(0x44)) {
    Serial.println("SHT31 sensor not found!");
  }

  // Initialize Button and Rain Sensor
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(rainSensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(rainSensorPin), rainPulseISR, FALLING);

  // Initialize ThingSpeak
  ThingSpeak.begin(espClient);
}

void loop() {
  unsigned long currentMillis = millis();

  // Read the current state of the button
  currentButtonState = digitalRead(buttonPin);

  // Detect valid button press (pressed and released)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    buttonHandled = true; // Button is pressed
  }

  if (currentButtonState == HIGH && lastButtonState == LOW && buttonHandled) {
    // Button released after being pressed
    buttonPressCount = (buttonPressCount + 1) % 5; // Cycle through messages
    displayMessage(buttonPressCount);
    lastButtonPress = currentMillis; // Record time of button press
    displayActive = true; // Keep OLED active
    buttonHandled = false; // Reset button handling flag
  }

  // Turn off OLED after timeout
  if (displayActive && currentMillis - lastButtonPress > displayTimeout) {
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    displayActive = false;
  }

  // Upload data every 1 minute
  if (currentMillis - lastUploadTime >= uploadInterval) {
    uploadDataToThingSpeak();
    lastUploadTime = currentMillis;
  }

  // Handle Rain Sensor Data
  handleRainSensor();

  // Update lastButtonState
  lastButtonState = currentButtonState;
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

void connectMQTT() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Connected to MQTT");
    } else {
      delay(5000);
    }
  }
}

void uploadDataToThingSpeak() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  float temperature = sht31.readTemperature();
  float humidity = sht31.readHumidity();

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, totalRainfall);

  int response = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (response == 200) {
    Serial.println("Data uploaded successfully!");
  } else {
    Serial.print("Error uploading data: ");
    Serial.println(response);
  }

  totalRainfall = 0.0; // Reset rain data
  rainPulses = 0;
}

void displayMessage(int index) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  switch (index) {
    case 1:
      u8g2.drawStr(0, 24, "Hi handsome!");
      u8g2.drawStr(0, 44, "(click the button)");
      break;
    case 2:
      u8g2.setCursor(0, 24);
      u8g2.print("Temperature: ");
      u8g2.print(sht31.readTemperature());
      u8g2.print(" C");
      break;
    case 3:
      u8g2.setCursor(0, 24);
      u8g2.print("RH: ");
      u8g2.print(sht31.readHumidity());
      u8g2.print(" %");
      break;
    case 4:
      u8g2.setCursor(0, 24);
      u8g2.print("Rain (24h): ");
      u8g2.print(totalRainfall);
      u8g2.print(" mm");
      break;
    default:
      u8g2.clearBuffer();
      break;
  }

  u8g2.sendBuffer();
}

void handleRainSensor() {
  int analogValue = analogRead(rainSensorPin);
  float voltage = (analogValue / 4095.0) * 3.3;

  if (voltage >= 2.5 && !wasHigh) {
    wasHigh = true;
  } else if (voltage <= 0.8 && wasHigh) {
    wasHigh = false;
    rainPulses++;
    totalRainfall += rainPerClick;
  }
}
