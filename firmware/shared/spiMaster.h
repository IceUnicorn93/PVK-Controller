//--------//
// MASTER //
//--------//

#include <SPI.h>
#include <driver/spi_master.h>
#include <driver/gpio.h>
#include <esp_timer.h>

// Gemeinsame Bus-Pins
#define GPIO_MOSI       23
#define GPIO_MISO       19
#define GPIO_SCLK       18
#define SENDER_HOST     SPI2_HOST

// Anzahl der Slaves
#define NUM_SLAVES      3

// Pro Slave: CS-Pin und Handshake-Pin
static const int GPIO_CS[NUM_SLAVES]        = { 5,  15, 16 };
static const int GPIO_HANDSHAKE[NUM_SLAVES] = { 4,  17, 21 };

// Pro Slave: Device-Handle und Semaphore
static spi_device_handle_t handle[NUM_SLAVES];
static SemaphoreHandle_t   rdySem[NUM_SLAVES];

// Buffers pro Slave
static char sendbuf[NUM_SLAVES][62] = {0};
static char recvbuf[NUM_SLAVES][62] = {0};

// ISR – der arg-Parameter enthält den Slave-Index
static void IRAM_ATTR gpio_handshake_isr_handler(void *arg) {
    int slaveIdx = (int)(intptr_t)arg;

    static uint32_t lasttime_us[NUM_SLAVES] = {0};
    uint32_t now = esp_timer_get_time();
    if (now - lasttime_us[slaveIdx] < 100) return;
    lasttime_us[slaveIdx] = now;

    BaseType_t mustYield = pdFALSE;
    xSemaphoreGiveFromISR(rdySem[slaveIdx], &mustYield);
    if (mustYield) portYIELD_FROM_ISR();
}

void DoSpiSetup() {
    // SPI-Bus einmalig initialisieren
    spi_bus_config_t buscfg = {};
    buscfg.mosi_io_num = GPIO_MOSI;
    buscfg.miso_io_num = GPIO_MISO;
    buscfg.sclk_io_num = GPIO_SCLK;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;

    esp_err_t ret = spi_bus_initialize(SENDER_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        Serial.println("SPI Bus Init fehlgeschlagen!");
        while (1) delay(1000);
    }

    gpio_install_isr_service(0);

    // Jeden Slave konfigurieren
    for (int i = 0; i < NUM_SLAVES; i++) {
        // Semaphore
        rdySem[i] = xSemaphoreCreateBinary();

        // Handshake-Pin
        gpio_config_t io_conf = {};
        io_conf.intr_type = GPIO_INTR_POSEDGE;
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
        io_conf.pin_bit_mask = (1ULL << GPIO_HANDSHAKE[i]);
        gpio_config(&io_conf);
        gpio_set_intr_type((gpio_num_t)GPIO_HANDSHAKE[i], GPIO_INTR_POSEDGE);
        // Slave-Index als Argument übergeben
        gpio_isr_handler_add((gpio_num_t)GPIO_HANDSHAKE[i],
                             gpio_handshake_isr_handler, (void*)(intptr_t)i);

        // SPI-Device mit eigenem CS-Pin
        spi_device_interface_config_t devcfg = {};
        devcfg.clock_speed_hz = 6 * 1000 * 1000;
        devcfg.duty_cycle_pos = 128;
        devcfg.mode = 0;
        devcfg.spics_io_num = GPIO_CS[i];
        devcfg.cs_ena_posttrans = 3;
        devcfg.queue_size = 3;

        ret = spi_bus_add_device(SENDER_HOST, &devcfg, &handle[i]);
        if (ret != ESP_OK) {
            Serial.printf("Slave %d hinzufuegen fehlgeschlagen!\n", i);
            while (1) delay(1000);
        }

        // Annehmen, dass Slave beim Start bereit ist
        xSemaphoreGive(rdySem[i]);
    }

    Serial.println("SPI Master bereit fuer alle Slaves.");
}

// Übertragung an einen bestimmten Slave
void DoSpiTransmission(int slaveIdx) {
    if (slaveIdx < 0 || slaveIdx >= NUM_SLAVES) return;

    spi_transaction_t t = {};
    t.length    = sizeof(sendbuf[slaveIdx]) * 8;
    t.tx_buffer = sendbuf[slaveIdx];
    t.rx_buffer = recvbuf[slaveIdx];

    xSemaphoreTake(rdySem[slaveIdx], portMAX_DELAY);

    esp_err_t ret = spi_device_transmit(handle[slaveIdx], &t);
    if (ret == ESP_OK) {
        Serial.printf("[Slave %d] ", slaveIdx);
        Serial.write(recvbuf[slaveIdx], t.rxlength / 8);
    }
}