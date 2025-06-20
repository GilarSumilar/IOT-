#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

const char* ssid = "KZ25-RX10023";
const char* password = "00000000";

const int LDR_DO_PIN = 34;
const int RELAY_PIN = 14;
const int ESP_BUILTIN_LED = 2;

bool manualMode = false;
bool relayState = false; // false = mati, true = nyala

void setup() {
  Serial.begin(115200);
  Serial.println("Sistem Lampu Otomatis Aktif (Menggunakan LDR DO)!");

  pinMode(LDR_DO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(ESP_BUILTIN_LED, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH); // Lampu mati
  digitalWrite(ESP_BUILTIN_LED, HIGH);

  Serial.print("Connecting to WiFi ");
  Serial.print(ssid);
  Serial.print("...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ESP_BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(ESP_BUILTIN_LED, HIGH);
    delay(500);
    Serial.print(".");
  }

  digitalWrite(ESP_BUILTIN_LED, LOW);
  Serial.println("");
  Serial.println("Connected to WiFi network!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/led", HTTP_GET, getLed);
  server.on("/led", HTTP_POST, setLedOtomatis);
  server.begin();
  Serial.println("Server Started...");
}

void loop() {
  server.handleClient();

  if (!manualMode) {
    int ldrDigitalStatus = digitalRead(LDR_DO_PIN);
    Serial.print("Status LDR (DO): ");
    Serial.println(ldrDigitalStatus == HIGH ? "TERANG" : "GELAP");

    if (ldrDigitalStatus == LOW) {
      digitalWrite(RELAY_PIN, LOW); // Lampu nyala
      relayState = true;
      Serial.println("Kondisi Gelap. Lampu MENYALA!");
    } else {
      digitalWrite(RELAY_PIN, HIGH); // Lampu mati
      relayState = false;
      Serial.println("Kondisi Terang. Lampu MATI.");
    }
  }
  delay(100);
}

void setLedOtomatis() {
  // Toggle manual mode dan relay
  manualMode = !manualMode;
  if (manualMode) {
    relayState = !relayState;
    digitalWrite(RELAY_PIN, relayState ? LOW : HIGH);
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", relayState ? "ON" : "OFF");
}

void getLed() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", relayState ? "ON" : "OFF");
}