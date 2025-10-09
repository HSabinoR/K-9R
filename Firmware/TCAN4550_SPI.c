#include "TCAN4550_SPI.h"

SPISettings tcanSPISettings(TCAN4550_SPI_CLK, MSBFIRST, SPI_MODE0);

// ======================================================
// Initialize SPI interface and chip select pin
// ======================================================
void tcan4550_SPI_begin(void) {
    pinMode(TCAN4550_CS_PIN, OUTPUT);
    digitalWrite(TCAN4550_CS_PIN, HIGH);
    SPI.begin();
}

// ======================================================
// Read 32-bit register from TCAN4550
// ======================================================
uint32_t tcan4550_read(uint16_t address) {
    uint16_t cmd = 0x2000 | ((address >> 2) & 0x0FFF);  // READ command
    uint8_t txBuf[6] = {
        (uint8_t)(cmd >> 8),
        (uint8_t)(cmd & 0xFF),
        0, 0, 0, 0
    };
    uint8_t rxBuf[6] = {0};

    SPI.beginTransaction(tcanSPISettings);
    digitalWrite(TCAN4550_CS_PIN, LOW);
    for (int i = 0; i < 6; i++) {
        rxBuf[i] = SPI.transfer(txBuf[i]);
    }
    digitalWrite(TCAN4550_CS_PIN, HIGH);
    SPI.endTransaction();

    uint32_t data = ((uint32_t)rxBuf[2] << 24) |
                    ((uint32_t)rxBuf[3] << 16) |
                    ((uint32_t)rxBuf[4] << 8)  |
                    (uint32_t)rxBuf[5];

    return data;
}

// ======================================================
// Write 32-bit register to TCAN4550
// ======================================================
void tcan4550_write(uint16_t address, uint32_t data) {
    uint16_t cmd = 0x1000 | ((address >> 2) & 0x0FFF);  // WRITE command
    uint8_t txBuf[6] = {
        (uint8_t)(cmd >> 8),
        (uint8_t)(cmd & 0xFF),
        (uint8_t)(data >> 24),
        (uint8_t)(data >> 16),
        (uint8_t)(data >> 8),
        (uint8_t)(data & 0xFF)
    };

    SPI.beginTransaction(tcanSPISettings);
    digitalWrite(TCAN4550_CS_PIN, LOW);
    for (int i = 0; i < 6; i++) {
        SPI.transfer(txBuf[i]);
    }
    digitalWrite(TCAN4550_CS_PIN, HIGH);
    SPI.endTransaction();
}

// ======================================================
// Modify specific bits in a 32-bit register (read-modify-write)
// ======================================================
void tcan4550_modify(uint16_t address, uint32_t mask, uint32_t value) {
    uint32_t reg = tcan4550_read(address);
    reg = (reg & ~mask) | (value & mask);
    tcan4550_write(address, reg);
}

// ======================================================
// Debug helper - print register name + value
// ======================================================
void tcan4550_printReg(uint16_t address, const char* name) {
    uint32_t val = tcan4550_read(address);
    Serial.print(name);
    Serial.print(": 0x");
    Serial.println(val, HEX);
}
