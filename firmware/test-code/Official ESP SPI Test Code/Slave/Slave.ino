//#include <SPI.h>
#include <driver/spi_slave.h>
//#include <driver/gpio.h>

// SLAVE

// Pin-Konfiguration
// Sichere Pins für ESP32 DevKit C V4
#define GPIO_HANDSHAKE  4
#define GPIO_MOSI       23
#define GPIO_MISO       19
#define GPIO_SCLK       18
#define GPIO_CS         5


#define RCV_HOST        SPI2_HOST
#define BUF_SIZE        129

// Callbacks für Handshake-Leitung
void IRAM_ATTR my_post_setup_cb(spi_slave_transaction_t *trans) {
    gpio_set_level((gpio_num_t)GPIO_HANDSHAKE, 1);
}

void IRAM_ATTR my_post_trans_cb(spi_slave_transaction_t *trans) {
    gpio_set_level((gpio_num_t)GPIO_HANDSHAKE, 0);
}

// DMA-fähige Buffer
char *sendbuf = nullptr;
char *recvbuf = nullptr;
int n = 0;

void setup() {
    Serial.begin(115200);

    // Handshake-Pin als Output konfigurieren
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << GPIO_HANDSHAKE);
    gpio_config(&io_conf);

    // Pull-ups auf SPI-Leitungen aktivieren
    gpio_set_pull_mode((gpio_num_t)GPIO_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode((gpio_num_t)GPIO_SCLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode((gpio_num_t)GPIO_CS, GPIO_PULLUP_ONLY);

    // SPI-Bus-Konfiguration
    spi_bus_config_t buscfg = {};
    buscfg.mosi_io_num = GPIO_MOSI;
    buscfg.miso_io_num = GPIO_MISO;
    buscfg.sclk_io_num = GPIO_SCLK;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;

    // SPI-Slave-Interface-Konfiguration
    spi_slave_interface_config_t slvcfg = {};
    slvcfg.mode = 0;
    slvcfg.spics_io_num = GPIO_CS;
    slvcfg.queue_size = 3;
    slvcfg.flags = 0;
    slvcfg.post_setup_cb = my_post_setup_cb;
    slvcfg.post_trans_cb = my_post_trans_cb;

    // SPI Slave initialisieren
    esp_err_t ret = spi_slave_initialize(RCV_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        Serial.println("SPI Slave Init fehlgeschlagen!");
        while (1) { delay(1000); }
    }

    // DMA-fähigen Speicher allokieren
    sendbuf = (char *)heap_caps_malloc(BUF_SIZE, MALLOC_CAP_DMA);
    recvbuf = (char *)heap_caps_malloc(BUF_SIZE, MALLOC_CAP_DMA);
    if (!sendbuf || !recvbuf) {
        Serial.println("DMA Buffer Allokierung fehlgeschlagen!");
        while (1) { delay(1000); }
    }

    Serial.println("SPI Slave bereit.");
}

void loop() {
    // Buffer vorbereiten
    memset(recvbuf, 0xA5, BUF_SIZE);
    snprintf(sendbuf, BUF_SIZE, "This is the receiver, sending data for transmission number %04d.", n);

    // Transaktion konfigurieren
    spi_slave_transaction_t t = {};
    t.length = 128 * 8;
    t.tx_buffer = sendbuf;
    t.rx_buffer = recvbuf;

    // Auf Master-Transaktion warten (blockiert bis Daten empfangen)
    esp_err_t ret = spi_slave_transmit(RCV_HOST, &t, portMAX_DELAY);
    if (ret == ESP_OK) {
        Serial.printf("Received: %s\n", recvbuf);
    }

    // Slave pausieren zum Stromsparen
    ret = spi_slave_disable(RCV_HOST);
    if (ret == ESP_OK) {
        Serial.println("slave paused ...");
    }

    delay(100);

    ret = spi_slave_enable(RCV_HOST);
    if (ret == ESP_OK) {
        Serial.println("slave ready !");
    }

    n++;
}
