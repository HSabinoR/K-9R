/*
 * TCAN4x5x_SPI.c
 * Description: This file is responsible for abstracting the lower-level microcontroller SPI read and write functions
 *
 *
 *
 * Copyright (c) 2019 Texas Instruments Incorporated.  All rights reserved.
 * Software License Agreement
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "TCAN4x5x_SPI.h"

static uint8_t _csPin;
static SPISettings _spiSettings;

void TCAN4x5x_SPI_Init(uint8_t csPin, uint32_t spiClock) {
    _csPin = csPin;
    _spiSettings = SPISettings(spiClock, MSBFIRST, SPI_MODE0);
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH);
    SPI.begin();
}

// -------------------------------------------------------------------
//  WRITE FUNCTIONS
// -------------------------------------------------------------------

void AHB_WRITE_32(uint16_t address, uint32_t data) {
    AHB_WRITE_BURST_START(address, 1);
    AHB_WRITE_BURST_WRITE(data);
    AHB_WRITE_BURST_END();
}

void AHB_WRITE_BURST_START(uint16_t address, uint8_t words) {
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);

    SPI.transfer(AHB_WRITE_OPCODE);
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);
    SPI.transfer(words);
}

void AHB_WRITE_BURST_WRITE(uint32_t data) {
    SPI.transfer((data >> 24) & 0xFF);
    SPI.transfer((data >> 16) & 0xFF);
    SPI.transfer((data >> 8) & 0xFF);
    SPI.transfer(data & 0xFF);
}

void AHB_WRITE_BURST_END(void) {
    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();
}

// -------------------------------------------------------------------
//  READ FUNCTIONS
// -------------------------------------------------------------------

uint32_t AHB_READ_32(uint16_t address) {
    uint32_t val;

    AHB_READ_BURST_START(address, 1);
    val = AHB_READ_BURST_READ();
    AHB_READ_BURST_END();

    return val;
}

void AHB_READ_BURST_START(uint16_t address, uint8_t words) {
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);

    SPI.transfer(AHB_READ_OPCODE);
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);
    SPI.transfer(words);
}

uint32_t AHB_READ_BURST_READ(void) {
    uint32_t val = 0;
    val |= ((uint32_t)SPI.transfer(0x00)) << 24;
    val |= ((uint32_t)SPI.transfer(0x00)) << 16;
    val |= ((uint32_t)SPI.transfer(0x00)) << 8;
    val |= ((uint32_t)SPI.transfer(0x00));
    return val;
}

void AHB_READ_BURST_END(void) {
    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();
}