#ifndef TCAN4550_SPI_H
#define TCAN4550_SPI_H

#include <stdint.h>
#include <Arduino.h>
#include <SPI.h>

// ===================== USER CONFIG =====================
#define TCAN4550_CS_PIN   6        // Chip Select pin
#define TCAN4550_SPI_CLK  4000000  // SPI clock frequency (Hz)

// ===================== FUNCTION PROTOTYPES =====================
void tcan4550_SPI_begin(void);

// Basic Register Access
uint32_t tcan4550_read(uint16_t address);
void     tcan4550_write(uint16_t address, uint32_t data);

// Bitfield (masked) operations
void     tcan4550_modify(uint16_t address, uint32_t mask, uint32_t value);

// Debug helpers
void     tcan4550_printReg(uint16_t address, const char* name);

#endif // TCAN4550_SPI_H