#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_SHT31.h>

// Initialize OLED Display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Initialize SHT31 Sensor
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// Pin Definitions
const int buttonPin = A4;       // Button to wake up OLED
const int rainSensorPin = A0;   // Rain sensor pin

// Variables
bool displayActive = false;     // Track if OLED is active
unsigned long displayStartTime = 0; // Track when OLED started displaying
const unsigned long displayDuration = 10000; // 10 seconds to display data

// Variables for data upload timing
unsigned long lastUploadTime = 0;
const unsigned long uploadInterval = 60000; // Upload data every 1 minute (60 seconds)

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);

  // Initialize button
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize rain sensor pin
  pinMode(rainSensorPin, INPUT);

  // Initialize SHT31 sensor
  if (!sht31.begin(0x44)) {
    Serial.println("SHT31 sensor not found!");
  }

  // Initialize OLED display
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 24, "Press Button to Wake");
  u8g2.sendBuffer();
}

void loop() {
  // Check if button is pressed
  if (digitalRead(buttonPin) == LOW && !displayActive) {
    displayActive = true; // Activate OLED
    displayStartTime = millis(); // Record start time
    displaySensorData(); // Display data immediately
  }

  // If OLED is active, check if the display duration has elapsed
  if (displayActive && millis() - displayStartTime >= displayDuration) {
    displayActive = false; // Deactivate OLED
    displayGoodbyeMessage(); // Display "Have a good day"
    enterDeepSleep(); // Enter deep sleep
  }

  // Upload sensor data every 1 minute
  if (millis() - lastUploadTime >= uploadInterval) {
    lastUploadTime = millis();
    uploadSensorData();
  }

  // Continuously update display if active
  if (displayActive) {
    displaySensorData();
  }
}

void displaySensorData() {
  // Read sensor data
  float temperature = sht31.readTemperature();
  float humidity = sht31.readHumidity();
  int rainValue = analogRead(rainSensorPin);

  // Display sensor data
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  if (!isnan(temperature)) {
    u8g2.setCursor(0, 20);
    u8g2.print("Temp: ");
    u8g2.print(temperature, 2);
    u8g2.print(" C");
  } else {
    u8g2.setCursor(0, 20);
    u8g2.print("Temp: Error");
  }

  if (!isnan(humidity)) {
    u8g2.setCursor(0, 40);
    u8g2.print("RH: ");
    u8g2.print(humidity, 2);
    u8g2.print(" %");
  } else {
    u8g2.setCursor(0, 40);
    u8g2.print("RH: Error");
  }

  u8g2.setCursor(0, 60);
  u8g2.print("Rain: ");
  u8g2.print(rainValue);

  u8g2.sendBuffer();
}

void displayGoodbyeMessage() {
  // Display "Have a good day"
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 30, "Have a good day!");
  u8g2.sendBuffer();
  delay(2000); // Display for 2 seconds before deep sleep
}

void uploadSensorData() {
  // Placeholder for uploading data
  float temperature = sht31.readTemperature();
  float humidity = sht31.readHumidity();
  int rainValue = analogRead(rainSensorPin);

  // Print the data to serial monitor (or integrate with ThingSpeak, etc.)
  Serial.println("Uploading Sensor Data...");
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Rain: ");
  Serial.println(rainValue);
}

void enterDeepSleep() {
  // Enter deep sleep mode
  Serial.println("Entering deep sleep...");
  u8g2.clearBuffer();
  u8g2.sendBuffer(); // Turn off OLED display
  ESP.deepSleep(0); // Deep sleep indefinitely until reset
}
