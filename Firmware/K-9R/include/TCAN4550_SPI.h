#ifndef TCAN4550_SPI_H
#define TCAN4550_SPI_H

#include <Arduino.h>
#include <SPI.h>

// SPI Command Words (from datasheet)
#define TCAN4550_CMD_READ_B_FL   0x41
#define TCAN4550_CMD_WRITE_B_FL  0x61

class TCAN4550_SPI {
public:
    TCAN4550_SPI(uint8_t csPin, uint32_t clock = 4000000);

    void begin();
    uint8_t transferByte(uint8_t data);

    // Register-level read/write
    uint32_t readRegister(uint16_t address);
    void writeRegister(uint16_t address, uint32_t value);
    void modifyRegister(uint16_t address, uint32_t mask, uint32_t value);

private:
    uint8_t _csPin;
    SPISettings _settings;

    void select();
    void deselect();
};

#endif