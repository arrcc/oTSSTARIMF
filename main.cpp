#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>

const char* ssid = "---";
const char* password = "---";

const char* serverPost = "http://192.168.1.2/insert_data.php";  // Replace with your local server IP
const char* serverGet = "http://192.168.1.2/get_warning_status.php?location_id=1";

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        // Simulated sensor data
        float flowRate = random(50, 200);  // Replace with actual sensor reading
        float rainIntensity = random(0, 50);  // Replace with actual sensor reading
        int locationID = 1;  // Change based on sensor's location

        // Generate timestamp using millis()
        unsigned long timestamp = millis() / 1000;  // Convert to seconds

        // 🔹 Display random values in Serial Monitor
        Serial.println("----- Simulated Sensor Data -----");
        Serial.print("Flow Rate: ");
        Serial.print(flowRate);
        Serial.println(" L/s");

        Serial.print("Rain Intensity: ");
        Serial.print(rainIntensity);
        Serial.println(" mm/hr");

        Serial.print("Timestamp: ");
        Serial.println(timestamp);
        Serial.println("---------------------------------");

        // Sending Data
        http.begin(serverPost);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        String postData = "location_id=" + String(locationID) + "&flow_rate=" + String(flowRate) +
                          "&rain_intensity=" + String(rainIntensity) + "&timestamp=" + String(timestamp);

        int httpResponseCode = http.POST(postData);

        if (httpResponseCode == 200) {  // Only proceed if the response is OK
            String response = http.getString();
            Serial.print(" Data Sent. Server Response: ");
            Serial.println(response);
        } else {
            Serial.print(" Error sending data: ");
            Serial.println(httpResponseCode);
        }
        http.end();

        delay(2000);  // Wait before checking flood warning

        // Retrieving Warning Status
        http.begin(serverGet);
        int httpCode = http.GET();

        if (httpCode == 200) {
            String warningResponse = http.getString();
            Serial.print(" Warning Status: ");
            Serial.println(warningResponse);
        } else {
            Serial.print(" Error fetching warning: ");
            Serial.println(httpCode);
        }
        http.end();
    } else {
        Serial.println("WiFi Disconnected. Retrying...");
        WiFi.begin(ssid, password);
    }

    delay(5000);  // Repeat every 5 seconds
}
