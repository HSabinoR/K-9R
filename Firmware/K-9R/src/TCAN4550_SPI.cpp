#include "TCAN4550_SPI.h"

TCAN4550_SPI::TCAN4550_SPI(uint8_t csPin, uint32_t clock)
: _csPin(csPin), _settings(clock, MSBFIRST, SPI_MODE0) {}

void TCAN4550_SPI::begin() {
    pinMode(_csPin, OUTPUT);
    deselect();
    SPI.begin();
}

void TCAN4550_SPI::select()   { digitalWrite(_csPin, LOW); }
void TCAN4550_SPI::deselect() { digitalWrite(_csPin, HIGH); }

uint8_t TCAN4550_SPI::transferByte(uint8_t data) {
    return SPI.transfer(data);
}

uint32_t TCAN4550_SPI::readRegister(uint16_t address) {
    uint32_t value = 0;
    SPI.beginTransaction(_settings);
    select();

    transferByte(TCAN4550_CMD_READ_B_FL);
    transferByte((address >> 8) & 0xFF);
    transferByte(address & 0xFF);
    transferByte(0x01);  // Number of words to transfer: 1 word = 4 bytes

    value |= ((uint32_t)transferByte(0x00)) << 24;
    value |= ((uint32_t)transferByte(0x00)) << 16;
    value |= ((uint32_t)transferByte(0x00)) << 8;
    value |= ((uint32_t)transferByte(0x00));

    deselect();
    SPI.endTransaction();
    return value;
}

void TCAN4550_SPI::writeRegister(uint16_t address, uint32_t value) {
    SPI.beginTransaction(_settings);
    select();

    transferByte(TCAN4550_CMD_WRITE_B_FL);
    transferByte((address >> 8) & 0xFF);
    transferByte(address & 0xFF);
    transferByte(0x01);  // 1 word = 4 bytes

    transferByte((value >> 24) & 0xFF);
    transferByte((value >> 16) & 0xFF);
    transferByte((value >> 8) & 0xFF);
    transferByte(value & 0xFF);

    deselect();
    SPI.endTransaction();
}

void TCAN4550_SPI::modifyRegister(uint16_t address, uint32_t mask, uint32_t value) {
    uint32_t regVal = readRegister(address);
    // Update only the bits specified by 'mask' with the corresponding bits from 'value'
    regVal = (regVal & ~mask) | (value & mask);
    writeRegister(address, regVal);
}
