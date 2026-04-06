#include <esp_now.h>
#include <WiFi.h>

// Gleiche Struktur wie beim Sender!
typedef struct nachricht_t {
  char text[32];
  int zahl;
} nachricht_t;

nachricht_t nachricht;

// Callback: wird aufgerufen, wenn Daten empfangen werden
void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *daten, int laenge) {
  memcpy(&nachricht, daten, sizeof(nachricht));

  Serial.printf("Nachricht von: %02X:%02X:%02X:%02X:%02X:%02X\n",
    info->src_addr[0], info->src_addr[1], info->src_addr[2],
    info->src_addr[3], info->src_addr[4], info->src_addr[5]);
  Serial.printf("Text: %s\n", nachricht.text);
  Serial.printf("Zahl: %d\n", nachricht.zahl);
  Serial.println("---");
  Serial.println(WiFi.macAddress());
  Serial.println("---");
}

void setup() {
  Serial.begin(115200);

  delay(2000);

  // WiFi im Station-Modus initialisieren
  WiFi.mode(WIFI_STA);

  // MAC-Adresse anzeigen (diese im Sender eintragen!)
  Serial.print("MAC-Adresse dieses Boards: ");
  Serial.println(WiFi.macAddress());

  // ESP-NOW initialisieren
  if (esp_now_init() != ESP_OK) {
    Serial.println("Fehler beim Initialisieren von ESP-NOW");
    return;
  }

  // Empfangs-Callback registrieren
  esp_now_register_recv_cb(onDataRecv);

  Serial.println("Empfänger bereit! Warte auf Nachrichten...");
}

void loop() {
  Serial.println("...");
  delay(500);
}