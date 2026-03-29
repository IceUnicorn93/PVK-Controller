#include <driver/spi_slave.h>
//#include <string.h>

// ── Konfiguration ──────────────────────────────────────────────
static constexpr size_t BUFFER_SIZE = 242;

// SPI-Pins (HSPI)
static constexpr int PIN_MOSI = 23;
static constexpr int PIN_MISO = 19;
static constexpr int PIN_SCLK = 18;
static constexpr int PIN_CS   = 5;

// DMA-fähige Puffer (müssen WORD-aligned sein)
WORD_ALIGNED_ATTR uint8_t rxBuffer[BUFFER_SIZE] = {};
WORD_ALIGNED_ATTR uint8_t txBuffer[BUFFER_SIZE] = {};

// ── Setup ──────────────────────────────────────────────────────
void setup()
{
    // Serial.begin(115200);
    // Serial.println("SPI Slave – wartet auf 242 Bytes …");

    // CS-Pin als Input konfigurieren
    pinMode(PIN_CS, INPUT);

    // SPI-Bus-Konfiguration
    spi_bus_config_t busConfig = {};
    busConfig.mosi_io_num   = PIN_MOSI;
    busConfig.miso_io_num   = PIN_MISO;
    busConfig.sclk_io_num   = PIN_SCLK;
    busConfig.quadwp_io_num = -1;
    busConfig.quadhd_io_num = -1;

    // Slave-Interface-Konfiguration
    spi_slave_interface_config_t slaveConfig = {};
    slaveConfig.mode          = 1;           // SPI-Mode 1 – kompensiert den MISO-Timing-Versatz des ESP32
    slaveConfig.spics_io_num  = PIN_CS;
    slaveConfig.queue_size    = 1;
    slaveConfig.flags         = 0;

    // SPI-Slave initialisieren (HSPI = SPI2_HOST)
    esp_err_t ret = spi_slave_initialize(SPI2_HOST, &busConfig, &slaveConfig, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        //Serial.printf("SPI-Slave Init fehlgeschlagen: %s\n", esp_err_to_name(ret));
        while (true) { delay(1000); }
    }

    //Serial.println("SPI-Slave bereit.");
}

// ── Loop ───────────────────────────────────────────────────────
void loop()
{
    // Transaktion vorbereiten
    spi_slave_transaction_t transaction = {};
    transaction.length    = BUFFER_SIZE * 8;   // Länge in Bits
    
    //txBuffer mit 5 füllen
    memset(txBuffer, 5, BUFFER_SIZE);

    transaction.rx_buffer = rxBuffer;
    transaction.tx_buffer = txBuffer;          // Antwort-Daten (hier leer)

    // Blockierend auf eine Übertragung vom Master warten
    esp_err_t ret = spi_slave_transmit(SPI2_HOST, &transaction, portMAX_DELAY);

    if (ret == ESP_OK) {
        // Serial.printf("Empfangen: %d Bits (%d Bytes)\n",
        //                transaction.trans_len, transaction.trans_len / 8);

        // // Empfangene Daten als Hex ausgeben
        // for (size_t i = 0; i < BUFFER_SIZE; i++) {
        //     Serial.printf("%02X ", rxBuffer[i]);
        //     if ((i + 1) % 16 == 0) Serial.println();
        // }
        // Serial.println();
    } else {
        //Serial.printf("SPI-Empfang Fehler: %s\n", esp_err_to_name(ret));
    }
}