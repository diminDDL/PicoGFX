#include "touch.hpp"

touch::touch(i2c_inst_t i2c_inst, uint32_t sda_pin, uint32_t scl_pin, uint32_t speed)
{
    this->i2c_inst = i2c_inst;
    this->sda_pin = sda_pin;
    this->scl_pin = scl_pin;
    this->speed = speed;
}

/**
 * @protected
 * @brief Initialize the touch controller
 */
void touch::initI2C()
{
    gpio_init(this->sda_pin);
    gpio_set_function(this->sda_pin, GPIO_FUNC_I2C);
    gpio_pull_up(this->sda_pin);

    gpio_init(this->scl_pin);
    gpio_set_function(this->scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(this->scl_pin);

    i2c_init(&this->i2c_inst, this->speed);
}

/**
 * @protected
 * @brief Read a single byte from the touch controller
 * @param addr I2C address
 * @param reg Register to read from
 * @param data Byte to read
 * @return length of the data read
 */
uint32_t touch::touchRead(uint8_t addr, uint8_t reg, uint8_t* data)
{
    return this->touchRead(addr, reg, data, 1);
}

/**
 * @protected
 * @brief Read data from the touch controller
 * @param addr I2C address
 * @param reg Register to read from
 * @param data Data to read
 * @param len Length of the data
 * @return length of the data read
 */
uint32_t touch::touchRead(uint8_t addr, uint8_t reg, uint8_t* data, size_t len)
{
    uint32_t bytes_written = 0;

    bytes_written = i2c_write_blocking(&this->i2c_inst, addr, &reg, 1, true);
    bytes_written += i2c_read_blocking(&this->i2c_inst, addr, data, len, false);

    return 0;
}

/**
 * @protected
 * @brief Write a single byte to the touch controller
 * @param addr I2C address
 * @param reg Register to write to
 * @param data Byte to write
 * @return length of the data written
 */
uint32_t touch::touchWrite(uint8_t addr, uint8_t reg, uint8_t data)
{
    uint8_t buf[2] = { reg, data };
    return i2c_write_blocking(&this->i2c_inst, addr, buf, 2, false);
}

/**
 * @protected
 * @brief Write data to the touch controller
 * @param addr I2C address
 * @param reg Register to write to
 * @param data Data to write
 * @param len Length of the data
 * @return length of the data written
 */
uint32_t touch::touchWrite(uint8_t addr, uint8_t reg, uint8_t* data, size_t len)
{
    uint32_t bytes_written = 0;

    bytes_written = i2c_write_blocking(&this->i2c_inst, addr, &reg, 1, true);
    bytes_written += i2c_write_blocking(&this->i2c_inst, addr, data, len, false);

    return bytes_written;
}

/**
 * @public
 * @brief Scan the I2C bus for devices
 */
void touch::i2cScan()
{
    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr)
    {
        if (addr % 16 == 0)
            printf("%02x ", addr);

        int ret;
        uint8_t rxdata;
        
        ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
}