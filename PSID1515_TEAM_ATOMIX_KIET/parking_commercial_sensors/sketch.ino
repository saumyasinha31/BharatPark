#include <WiFi.h>
#include <HTTPClient.h>

#define FIREBASE_HOST "qrcode-9211.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyCs_5MjkLI1_6CqVoc275l-ckbJM7mjG8M"

struct DeviceData {
  int distance;
  // Add other fields as needed
};

DeviceData deviceData;

const int trigPin = 12;
const int echoPin = 14;

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  WiFi.begin("Wokwi-GUEST", "");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void loop() {
  deviceData.distance = getUltrasonicDistance();
  sendToFirebase(deviceData);

  delay(60000);
}

void sendToFirebase(DeviceData data) {
  String path = "/devices/device1"; 

  // Construct the URL for the Firebase Realtime Database
  String url = "https://qrcode-9211-default-rtdb.asia-southeast1.firebasedatabase.app/";

  // Construct the JSON payload
  String payload = "{\"distance\":" + String(data.distance) + "}";

  // Send HTTP POST request to Firebase
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(payload);
  http.end();

  Serial.println("Firebase HTTP Response Code: " + String(httpResponseCode));
}

int getUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  return distance;
}
