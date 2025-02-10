#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <hardware/i2c.h>
#include "pico/stdlib.h"

class touch
{
public:
    touch(i2c_inst_t i2c_inst, uint32_t sda_pin, uint32_t scl_pin, uint32_t speed = 400'000);
    void i2cScan();

protected:
    i2c_inst_t i2c_inst;
    uint32_t sda_pin;
    uint32_t scl_pin;
    uint32_t speed;

    void initI2C();

    uint32_t touchRead(uint8_t addr, uint8_t reg, uint8_t* data);
    uint32_t touchRead(uint8_t addr, uint8_t reg, uint8_t* data, size_t len);
    uint32_t touchWrite(uint8_t addr, uint8_t reg, uint8_t data);
    uint32_t touchWrite(uint8_t addr, uint8_t reg, uint8_t* data, size_t len);
};