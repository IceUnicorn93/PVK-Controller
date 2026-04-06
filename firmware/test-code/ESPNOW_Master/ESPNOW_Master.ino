#include <esp_now.h>
#include <WiFi.h>

// MAC-Adresse des Empfängers hier eintragen!
// Du findest sie mit dem Empfänger-Sketch im Serial Monitor.
uint8_t empfaengerMAC[] = {0xB0,0xCB,0xD8,0x89,0xDA,0x08};

// Struktur für die zu sendenden Daten
typedef struct nachricht_t {
  char text[32];
  int zahl;
} nachricht_t;

nachricht_t nachricht;
// Callback: wird aufgerufen, wenn Daten gesendet wurden
// WICHTIG: In ESP32-Arduino-Core v3.x ist der erste Parameter wifi_tx_info_t*
void onDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  Serial.print("Sendestatus: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Erfolgreich" : "Fehlgeschlagen");
}

void setup() {
  Serial.begin(115200);

  // WiFi im Station-Modus initialisieren
  WiFi.mode(WIFI_STA);

  // ESP-NOW initialisieren
  if (esp_now_init() != ESP_OK) {
    Serial.println("Fehler beim Initialisieren von ESP-NOW");
    return;
  }

  // Sende-Callback registrieren
  esp_now_register_send_cb(onDataSent);

  // Empfänger als Peer hinzufügen
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, empfaengerMAC, 6);
  peerInfo.channel = 0;      // Standard-Kanal
  peerInfo.encrypt = false;   // Keine Verschlüsselung

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Fehler beim Hinzufügen des Peers");
    return;
  }

  Serial.println("Sender bereit!");
}

void loop() {
  // Nachricht vorbereiten
  strcpy(nachricht.text, "Hallo ESP-NOW!");
  nachricht.zahl = millis() / 1000; // Sekunden seit Start

  // Nachricht senden
  esp_err_t ergebnis = esp_now_send(empfaengerMAC, (uint8_t *)&nachricht, sizeof(nachricht));

  if (ergebnis == ESP_OK) {
    Serial.printf("Nachricht gesendet: \"%s\", Zahl: %d\n", nachricht.text, nachricht.zahl);
  } else {
    Serial.println("Fehler beim Senden");
  }

  delay(2000); // Alle 2 Sekunden senden
}