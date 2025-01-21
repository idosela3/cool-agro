// Pin configuration
#define RAIN_SENSOR_PIN 36  // A0 (GPIO36) for ADC input
#define ADC_MAX 4095        // Maximum ADC value for 12-bit resolution
#define V_REF 3.3           // Reference voltage of ESP32 (3.3V)
#define HIGH_THRESHOLD 2.5  // Threshold voltage to consider "high" (near 3.3V)
#define LOW_THRESHOLD 0.8   // Threshold voltage to consider "low" (near 0V)

// Rainfall calculation
const float rainPerClick = (50.0 / 24.0) * (10.0 / 55.0); // mm per click
volatile int clickCount = 0;   // Counts the number of clicks
float totalRainfall = 0.0;     // Total rainfall in mm

// Timing
unsigned long startTime = 0;  // Start time for the 30-second interval
const unsigned long interval = 30000; // 30 seconds in milliseconds

// Tracks the last voltage state
bool wasHigh = false;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Configure rain sensor pin as input
  pinMode(RAIN_SENSOR_PIN, INPUT);

  Serial.println("Rainfall measurement system initialized.");
  startTime = millis(); // Initialize the start time
}

void loop() {
  // Read the analog value from the rain sensor pin
  int analogValue = analogRead(RAIN_SENSOR_PIN);

  // Convert the analog value to voltage
  float voltage = (analogValue / (float)ADC_MAX) * V_REF;

  // Detect clicks based on voltage transitions
  if (voltage >= HIGH_THRESHOLD && !wasHigh) {
    // Voltage transitions from low to high
    wasHigh = true;
  } else if (voltage <= LOW_THRESHOLD && wasHigh) {
    // Voltage transitions from high to low and completes a cycle
    wasHigh = false;
    clickCount++;

    // Update total rainfall
    totalRainfall += rainPerClick;
  }

  // Check if 30 seconds have elapsed
  unsigned long currentTime = millis();
  if (currentTime - startTime >= interval) {
    // Display the total rainfall for the last 30 seconds
    Serial.print("Rainfall in the last 30 seconds: ");
    Serial.print(totalRainfall, 2); // Print rainfall with 2 decimal places
    Serial.println(" mm");

    // Reset for the next interval
    totalRainfall = 0.0;
    clickCount = 0;
    startTime = currentTime; // Reset the timer
  }

  // Small delay for stability
  delay(10);
}
