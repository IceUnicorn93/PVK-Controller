//--------//
// MASTER //
//--------//

#include <SPI.h>
#include <driver/spi_master.h>
#include <driver/gpio.h>
#include <esp_timer.h>

// Pin-Konfiguration
// Sichere Pins für ESP32 DevKit C V4
#define GPIO_HANDSHAKE  4
#define GPIO_MOSI       23
#define GPIO_MISO       19
#define GPIO_SCLK       18
#define GPIO_CS         5


#define SENDER_HOST     SPI2_HOST

// Semaphore: signalisiert, dass der Slave bereit ist
static SemaphoreHandle_t rdySem;

// ISR für die Handshake-Leitung
static void IRAM_ATTR gpio_handshake_isr_handler(void *arg) {
    static uint32_t lasthandshaketime_us = 0;
    uint32_t currtime_us = esp_timer_get_time();
    uint32_t diff = currtime_us - lasthandshaketime_us;
    if (diff < 100) {
        return; // Interrupts < 100us nach dem letzten ignorieren
    }
    lasthandshaketime_us = currtime_us;

    BaseType_t mustYield = pdFALSE;
    xSemaphoreGiveFromISR(rdySem, &mustYield);
    if (mustYield) {
        portYIELD_FROM_ISR();
    }
}

spi_device_handle_t handle;
int n = 0;
char sendbuf[62] = {0};
char recvbuf[62] = {0};

void DoSpiSetup()
{
	// Semaphore erstellen
    rdySem = xSemaphoreCreateBinary();

    // Handshake-Pin als Input mit Interrupt auf steigende Flanke
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pin_bit_mask = (1ULL << GPIO_HANDSHAKE);
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_set_intr_type((gpio_num_t)GPIO_HANDSHAKE, GPIO_INTR_POSEDGE);
    gpio_isr_handler_add((gpio_num_t)GPIO_HANDSHAKE, gpio_handshake_isr_handler, NULL);

    // SPI-Bus-Konfiguration
    spi_bus_config_t buscfg = {};
    buscfg.mosi_io_num = GPIO_MOSI;
    buscfg.miso_io_num = GPIO_MISO;
    buscfg.sclk_io_num = GPIO_SCLK;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;

    // SPI-Device-Konfiguration
    spi_device_interface_config_t devcfg = {};
    devcfg.command_bits = 0;
    devcfg.address_bits = 0;
    devcfg.dummy_bits = 0;
    devcfg.clock_speed_hz = 6 * 1000 * 1000; // 3MHz
    devcfg.duty_cycle_pos = 128;        // 50% Duty Cycle
    devcfg.mode = 0;
    devcfg.spics_io_num = GPIO_CS;
    devcfg.cs_ena_posttrans = 3;        // CS 3 Zyklen nach Transaktion low halten
    devcfg.queue_size = 3;

    // SPI Bus initialisieren und Device hinzufügen
    esp_err_t ret = spi_bus_initialize(SENDER_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        Serial.println("SPI Bus Init fehlgeschlagen!");
        while (1) { delay(1000); }
    }

    ret = spi_bus_add_device(SENDER_HOST, &devcfg, &handle);
    if (ret != ESP_OK) {
        Serial.println("SPI Device hinzufuegen fehlgeschlagen!");
        while (1) { delay(1000); }
    }

    // Annehmen, dass der Slave beim ersten Mal schon bereit ist
    xSemaphoreGive(rdySem);

    Serial.println("SPI Master bereit.");
}

void DoSpiTransmission()
{
	// int res = snprintf(sendbuf, sizeof(sendbuf), "sender, transmission no. %04i. last time, i received: \"%s\"", n, recvbuf);
	// if (res >= (int)sizeof(sendbuf)) {
	// // serial.println("data truncated");
	// }

    spi_transaction_t t = {};
    t.length = sizeof(sendbuf) * 8;
    t.tx_buffer = sendbuf;
    t.rx_buffer = recvbuf;

    // Warten bis der Slave bereit ist
    xSemaphoreTake(rdySem, portMAX_DELAY);

    esp_err_t ret = spi_device_transmit(handle, &t);
    if (ret == ESP_OK) {
        Serial.write(recvbuf, t.rxlength / 8); // rxlength ist in Bits
    } else {
     // serial.printf("transmit fehler: %d\n", ret);
    }

    // n++;
}