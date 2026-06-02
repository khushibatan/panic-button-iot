#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
const int buttonPin = D6;
const int ledPin = D8;
const int buzzerPin = D5;

// 🛜 WiFi Credentials
const char* ssid = "wifi";             // <-- Replace with your WiFi SSID
const char* password = "pswd";     // <-- Replace with your WiFi Password

// 🤖 Telegram Bot Credentials
const char* botToken = "token";         // <-- Replace with your Bot Token
String chatID = "chatid";                  // <-- Replace with your Telegram Chat ID

// 🔘 Push Button Configuration
//const int buttonPin = D6;                        // <-- Change pin if using another
bool lastButtonState = HIGH;

void sendTelegramMessage(String message);

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);              // Use internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  // Detect falling edge (button press)
  if (buttonState == LOW && lastButtonState == HIGH) {
   Serial.println("🔘 Button Pressed!");
   digitalWrite(ledPin, HIGH);
   digitalWrite(buzzerPin, HIGH);
   sendTelegramMessage("Panic Button Pressed");
   delay(1000);
   digitalWrite(ledPin, LOW);
   digitalWrite(buzzerPin, LOW);
  }
  lastButtonState = buttonState;
}

void sendTelegramMessage(String message) {
  WiFiClientSecure client;
  client.setInsecure();

  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("❌ Connection to Telegram failed");
    return;
  }

  // Replace spaces with URL-safe text
  message.replace(" ", "%20");

  String url = "/bot" + String(botToken) +
               "/sendMessage?chat_id=" + chatID +
               "&text=" + message;

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.telegram.org\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("✅ Telegram request sent!");
}