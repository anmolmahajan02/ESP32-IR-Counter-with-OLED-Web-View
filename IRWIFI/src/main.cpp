//VCC-->3V3
//SDA-->21
//SCK-->22

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1

int irPin = 23;         // IR sensor connected to GPIO 23
int count = 0;          // Counter for object passes
bool objectDetected = false;
int touchPin = 4;

const char *ssid = "MyESP";
const char *password = "9103157011";
WiFiServer server(80);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  pinMode(irPin, INPUT);    // Set IR pin as input
  pinMode(touchPin, INPUT);

  Wire.begin(21, 22); // SDA, SCL for ESP32
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1);
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 initialization failed"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("IR Counter Ready");
  display.display();
  delay(1000);
}

void loop() {
  int sensorValue = digitalRead(irPin);

  // Object detected (IR beam broken)
  if (sensorValue == LOW && !objectDetected) {
    count++;
    objectDetected = true;
    Serial.print("Object Passed. Count: ");
    Serial.println(count);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Count:");
    display.setTextSize(3);
    display.setCursor(32, 32);
    display.println(count);
    display.display();

    
  }

  // Reset detection when object moves away
  if (sensorValue == HIGH) {
    objectDetected = false;
  }

  if (touchRead(touchPin) < 30) {
  count = 0;
  Serial.println("Count Reset by Touch!");

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Count:");
  display.setTextSize(3);
  display.setCursor(32, 32);
  display.println(count);
  display.display();

  delay(500); // debounce delay to prevent multiple resets
}

  delay(30); // Debounce delay

  WiFiClient client = server.accept();  // listen for incoming clients

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {

          // Handle /count endpoint
          if (currentLine.startsWith("GET /count")) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Connection: close");
            client.println();
            client.println(count);
            break;
          }

          // Serve main page
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println(R"rawliteral(
              <!DOCTYPE html>
              <html>
              <head>
                <meta charset="UTF-8">
                <title>IR Counter</title>
                <style>
                  body { font-family: sans-serif; text-align: center; padding: 50px; }
                  #count { font-size: 60px; color: green; }
                </style>
              </head>
              <body>
                <h1>Live IR Counter</h1>
                <div id="count">0</div>

                <script>
                  function fetchCount() {
                    fetch('/count')
                      .then(response => response.text())
                      .then(data => {
                        document.getElementById('count').innerText = data;
                      });
                  }

                  setInterval(fetchCount, 200); // Every 1 second
                  fetchCount(); // Call immediately on page load
                </script>
              </body>
              </html>
            )rawliteral");

            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
