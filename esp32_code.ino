#include <WiFi.h>
#include <FirebaseESP32.h>

// 🔥 Firebase Configuration (Replace with your credentials)
#define FIREBASE_HOST "wildlife-poaching-alerts-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCXESgCpdwJjk6l\nmwtSZEf0/qwqetkNappjzUtPLlM+XvDpu66JpcLgXm7OwzMAKIL6J1r2l136nqDZ\nhGMyOJx5Zqe82p3FDJ2RblE8xXyeqZXXH5IrjXX0X1uxY6WFavv0m6Zgn0eL7MV7\nMh5VPjc3uMYj0bI7iokgbhXlZ7fxUlMf5JhpyumtzVEOj4JBQEsHtBqU1rGtEz6E\nHV64dRmaC3RyKOjdu2ffrVH0LD11+LnqmPxf35KRjUE0w8vNsV45qbs+oW35D2Ky\nd3zETVQmnnwMPup3a/htZCoOhZ5iD/J0sj7vte8+ya2dZxfHXE0tLCMnSemw9zgw\nT9/KyTYNAgMBAAECggEAA3xGbsyKXOBpwUmS6OH+RLFlN1lcbYI75oPA/NDzY+aX\nYDaFF/vD2QmSEhcFXnfQ0STmz6Ey7np0o3LPQhmgce7z0vg9Tc5VIAWfqLYKwnNk\nYmasqJtr27P2iYt5xnAEjlfSrnsw+1IUGrFPlgUTNj0pudUCVnwuCUTuSuZ0ze51\n+5U5CUrKNdmwcskyIDUqXaXmiwaVp0GGHpQL07toJ6tpp/KXE2y4I1MOWAQJmM5X\n2Mlj+PmEVfMTO+TcXi8KfdFh46YvBAkszFuCP0sucBxJXeshxInPkBLfD1yuDqnL\nq0yPPU/AMrRmlX5Krxsf5YhdQvbQcNXzkcQJOTjwaQKBgQDRgdvGmLTXUS7GsTUX\nN78K841npSKMd/xvvJZicKSvlr1HPbsNMd1NstPYpnT/g7i2eILi2Jekkytla6AV\narCWzzJ3B/ZBCcoSvnK9FI9tGbwELOfTPjOvJVEnEWxxPvIi5q3qMovK92yK/Uas\nEyRkqRL+XaqNbOdtYnSxNOb6WQKBgQC4l0xPFY5q2u+rrd/mbtNdJU3uRn36/kok\nRi4ad9QLh2uxaWEV+sToaChFGpQFoyOr81ZKykjLTm9tml4esqzqirmuyMr5/me5\nFbM9bWMd9Cu1rPHQ638fU/44odwkZv1OBaWn7kc3xS5xhkIqpw8tJ+H8XgvCMGJm\nW04R4yD61QKBgCfzPEZtfiaE24eV6QrJXYuoD7HQ4nqosqFX89FHq4+0DdnU4qNM\ne+fCDvWEIFFM+98HseWx6ZC/Y4oOcNNEqfHIBtZ91S5nwHm8iUKW9haWzqx5J6e3\nd8GROx1ZDDW4DBItrMI460FD1xXV4DEV4V20g+OvVsNrHeJU4O2dqeHZAoGBAKtY\n7qHerbPtBVuE7Hj32UX2ke+kCd+KWwJvDu7M2s3U6BfobRG9FTlzLiZvdIYDGOLN\n4AVO3uGyLmc8SzANxct1+MURaLFFAb1iMl7MApzUghqDrnizke5fxnWFcldsLqhV\nP5+x5PQVGnaRHSyApOrSwcjE9/9LcJv5F3AtOPj9AoGAZowxg42iV+VsHDJ7CHMM\nr0ZP7jwRAarD7zUSEdXsO5HiDMi0w3UJtQUsgzIg1oi2E+0T7TpxrqANH3P8jA+Z\nP2zYPAkDokSrMpvlKvgO+MXADG+CLV1teOlKhtcWaE4pBqqOQJPYd/noNJBCLatM\nqIdTPxFOH4930GN/oHRLwks=\n-----END PRIVATE KEY-----\n"


const char* ssid = "your-SSID";         // Replace with your WiFi SSID
const char* password = "your-PASSWORD"; // Replace with your WiFi Password

FirebaseData firebaseData;

// 🚨 Sensor Pins
#define PIR_PIN 4
#define MIC_PIN 5
#define VIBRATION_PIN 6
#define ULTRASONIC_TRIG 7
#define ULTRASONIC_ECHO 8

// 🌍 System Variables
bool motionDetected = false;
bool gunshotDetected = false;
bool poacherAttackDetected = false;
bool animalEscaped = false;

void setup() {
    Serial.begin(115200);

    // 🌐 Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\n✅ WiFi Connected!");

    // 🔥 Initialize Firebase
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    // 📌 Set Pin Modes
    pinMode(PIR_PIN, INPUT);
    pinMode(MIC_PIN, INPUT);
    pinMode(VIBRATION_PIN, INPUT);
    pinMode(ULTRASONIC_TRIG, OUTPUT);
    pinMode(ULTRASONIC_ECHO, INPUT);
}

void loop() {
    // 🏕️ **Zone 1 - Motion Detection (Poacher Intrusion)**
    motionDetected = digitalRead(PIR_PIN);
    if (motionDetected) {
        sendAlert("/alerts/zone1", "⚠️ Intruder detected in Zone 1!");
    }

    // 🎯 **Zone 2 - Gunshot Detection**
    gunshotDetected = digitalRead(MIC_PIN);
    if (gunshotDetected) {
        sendAlert("/alerts/zone2", "🔫 Gunshot detected in Zone 2!");
    }

    // 🐆 **Zone 3 - Animal Escape Detection**
    int distance = getUltrasonicDistance();
    if (distance > 0 && distance < 30) { // If animal crosses boundary (30cm threshold)
        sendAlert("/alerts/zone3", "⚠️ Animal escaped from Zone 3!");
    }

    // 🎭 **Zone 4 - Poaching Attempt Detection (Fake Animal Vibration)**
    poacherAttackDetected = digitalRead(VIBRATION_PIN);
    if (poacherAttackDetected) {
        sendAlert("/alerts/zone4", "🚨 Poacher attack detected in Zone 4!");
    }

    // 🎥 **Zone 5 - ESP32-CAM Surveillance (Handled Separately in ESP32-CAM)**
    sendAlert("/alerts/zone5", "📸 Camera monitoring active in Zone 5.");

    delay(3000); // Reduce delay for faster updates
}

// 📏 **Function to Get Distance from Ultrasonic Sensor**
int getUltrasonicDistance() {
    digitalWrite(ULTRASONIC_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(ULTRASONIC_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_TRIG, LOW);
    
    int duration = pulseIn(ULTRASONIC_ECHO, HIGH);
    int distance = duration * 0.034 / 2; // Convert to cm

    if (distance > 400 || distance <= 0) {
        return -1; // Invalid reading
    }
    return distance;
}

// 🔥 **Function to Send Alerts to Firebase**
void sendAlert(const char* path, String message) {
    if (Firebase.setString(firebaseData, path, message)) {
        Serial.println("✅ Firebase Updated: " + message);
    } else {
        Serial.println("❌ Firebase Error: " + firebaseData.errorReason());
    }
}
