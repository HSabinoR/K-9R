#include "TCAN4550_SPI.h"

static uint8_t _tcan4550_csPin;
static SPISettings _tcan4550_settings;

void TCAN4550_SPI_Begin(uint8_t csPin, uint32_t clock) {
    _tcan4550_csPin = csPin;
    _tcan4550_settings = SPISettings(clock, MSBFIRST, SPI_MODE0);

    pinMode(_tcan4550_csPin, OUTPUT);
    digitalWrite(_tcan4550_csPin, HIGH);
    SPI.begin();
}

static inline void selectChip()   { digitalWrite(_tcan4550_csPin, LOW); }
static inline void deselectChip() { digitalWrite(_tcan4550_csPin, HIGH); }

uint8_t TCAN4550_TransferByte(uint8_t data) {
    return SPI.transfer(data);
}

uint32_t TCAN4550_ReadRegister(uint16_t address) {
    uint32_t value = 0;
    SPI.beginTransaction(_tcan4550_settings);
    selectChip();

    TCAN4550_TransferByte(TCAN4550_CMD_READ_B_FL);
    TCAN4550_TransferByte((address >> 8) & 0xFF);
    TCAN4550_TransferByte(address & 0xFF);
    TCAN4550_TransferByte(0x01); // 1 word = 4 bytes = 32 bits

    value |= ((uint32_t)TCAN4550_TransferByte(0x00)) << 24;
    value |= ((uint32_t)TCAN4550_TransferByte(0x00)) << 16;
    value |= ((uint32_t)TCAN4550_TransferByte(0x00)) << 8;
    value |= ((uint32_t)TCAN4550_TransferByte(0x00));

    deselectChip();
    SPI.endTransaction();
    return value;
}

void TCAN4550_WriteRegister(uint16_t address, uint32_t value) {
    SPI.beginTransaction(_tcan4550_settings);
    selectChip();

    TCAN4550_TransferByte(TCAN4550_CMD_WRITE_B_FL);
    TCAN4550_TransferByte((address >> 8) & 0xFF);
    TCAN4550_TransferByte(address & 0xFF);
    TCAN4550_TransferByte(0x01); // 1 word = 4 bytes

    TCAN4550_TransferByte((value >> 24) & 0xFF);
    TCAN4550_TransferByte((value >> 16) & 0xFF);
    TCAN4550_TransferByte((value >> 8) & 0xFF);
    TCAN4550_TransferByte(value & 0xFF);

    deselectChip();
    SPI.endTransaction();
}

void TCAN4550_ModifyRegister(uint16_t address, uint32_t mask, uint32_t value) {
    uint32_t regVal = TCAN4550_ReadRegister(address);
    regVal = (regVal & ~mask) | (value & mask);
    TCAN4550_WriteRegister(address, regVal);
}
