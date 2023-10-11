#include <Blynk.h>
#define BLYNK_TEMPLATE_ID "TMPL3sHyzcKwJ"
#define BLYNK_TEMPLATE_NAME "Air Pollution Monitoring"
#define BLYNK_AUTH_TOKEN "9xq9oIy3rA_jvP4kH3oXKXrDpwtDXbZ8"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Analog input pins for gas sensors
int MQ4 = 36;
int MQ7 = 39;
int MQ135 = 34;
int MQ2 = 35;

// Air quality index (AQI) variables for each gas sensor
int AQI_MQ135;
int AQI_MQ2;
int AQI_MQ4;
int AQI_MQ7;

// Threshold values for each gas sensor
int THRESHOLD_MQ135 = 500;
int THRESHOLD_MQ2 = 2000;
int THRESHOLD_MQ4 = 2000;
int THRESHOLD_MQ7 = 700;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Jhreddy";    // type your wifi name
char pass[] = "123456789";  // type your wifi password

BlynkTimer timer;

void sendSensor() {
  int analog_data_MQ135 = analogRead(MQ135);
  int analog_data_MQ2 = analogRead(MQ2);
  int analog_data_MQ4 = analogRead(MQ4);
  int analog_data_MQ7 = analogRead(MQ7);

  // Calculate air quality index (AQI) for each gas sensor
  AQI_MQ135 = map(analog_data_MQ135, 0, 1023, 10, 1000);  // Change the scaling values based on your calibration carbon Dioxide - 350 to 10000, Ammonia (NH3)- 10 to 300, Nitrogen oxides(NOx) - 10 to 100
  AQI_MQ2 = map(analog_data_MQ2, 0, 1023, 200, 10000);    // Change the scaling values based on your calibration. LPG, propane - 1000 to 10000
  AQI_MQ4 = map(analog_data_MQ4, 0, 1023, 300, 10000);    // Change the scaling values based on your calibration Methane,Natural gas(CNG)
  AQI_MQ7 = map(analog_data_MQ7, 0, 1023, 20, 2000);      // Change the scaling values based on your calibration Carbon Monoxide

  // Send AQI values to Blynk server
  Blynk.virtualWrite(V0, AQI_MQ135);
  Blynk.virtualWrite(V1, AQI_MQ2);
  Blynk.virtualWrite(V2, AQI_MQ4);
  Blynk.virtualWrite(V3, AQI_MQ7);

  // Check if AQI values exceed the threshold for each gas sensor
  if (AQI_MQ135 > THRESHOLD_MQ135) {
    Blynk.logEvent("MQ135_Event");
  }
  if (AQI_MQ2 > THRESHOLD_MQ2) {
    Blynk.logEvent("MQ2_Event");
  }
  if (AQI_MQ4 > THRESHOLD_MQ4) {
    Blynk.logEvent("Air quality alert: High Methane/Natural gas(CNG) levels detected!");
  }
  if (AQI_MQ7 > THRESHOLD_MQ7) {
    Blynk.logEvent("MQ7_Event");
  }
}

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Connect to WiFi network
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to Blynk server
  Blynk.begin(auth, ssid, pass);

  // Setup timer to send sensor data every 1 second
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  // Run Blynk loop
  Blynk.run();

  // Run timer loop
  timer.run();
}