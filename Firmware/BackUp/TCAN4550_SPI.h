#ifndef TCAN4550_SPI_H
#define TCAN4550_SPI_H

#include <Arduino.h>
#include <SPI.h>

// SPI command words
#define TCAN4550_CMD_READ_B_FL   0x41
#define TCAN4550_CMD_WRITE_B_FL  0x61

// Initializes the SPI interface and stores the chip select pin
void TCAN4550_SPI_Begin(uint8_t csPin, uint32_t clock = 4000000);

// Transfers a single byte over SPI
uint8_t TCAN4550_TransferByte(uint8_t data);

// Register-level operations
uint32_t TCAN4550_ReadRegister(uint16_t address);
void TCAN4550_WriteRegister(uint16_t address, uint32_t value);
void TCAN4550_ModifyRegister(uint16_t address, uint32_t mask, uint32_t value);

#endif
