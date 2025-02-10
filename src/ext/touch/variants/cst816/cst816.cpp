#include "cst816.hpp"

cst816* cst816::instance = nullptr;  // Define the static instance

/**
 * @public
 * @brief CST816 constructor
 * @param i2c_inst I2C instance
 * @param sda_pin SDA pin
 * @param scl_pin SCL pin
 * @param irq_pin IRQ pin
 * @param rst_pin RST pin
 * @note This constructor will setup a IRQ handler for the CST816 as the CST816 will not respond unless touch is detected!
 */
cst816::cst816(i2c_inst_t i2c_inst, uint32_t sda_pin, uint32_t scl_pin, uint32_t irq_pin, uint32_t rst_pin) : touch(i2c_inst, sda_pin, scl_pin)
{
    this->irq_pin = irq_pin;
    this->rst_pin = rst_pin;
    this->instance = this;
}

/**
 * @public
 * @brief Init the CST816
 */
void cst816::init()
{
    this->initI2C();

    gpio_init(this->irq_pin);
    gpio_set_dir(this->irq_pin, GPIO_IN);
    gpio_pull_up(this->irq_pin);

    gpio_init(this->rst_pin);
    gpio_set_dir(this->rst_pin, GPIO_OUT);
    this->reset();

    gpio_set_irq_enabled_with_callback(this->irq_pin, GPIO_IRQ_EDGE_FALL, true, &cst816::static_irq_handler);
}

/**
 * @public
 * @brief Reset the CST816
 */
void cst816::reset()
{
    gpio_put(this->rst_pin, 1);
    sleep_ms(50);
    gpio_put(this->rst_pin, 0);
    sleep_ms(5);
    gpio_put(this->rst_pin, 1);
}

/**
 * @public
 * @brief Disable auto sleep on the CST816
 */
void cst816::disableStandby()
{
    this->touchWrite(CST816_ADDR, 0xfe, 0xfe);
}

/**
 * @public
 * @brief Enable auto sleep on the CST816
 */
void cst816::enableStandby()
{
    this->touchWrite(CST816_ADDR, 0xfe, 0x00);
}

/**
 * @public
 * @brief Check if data is available
 * @returns True if data is available
 * @note Returns true only once per call and is reset when new data is available
 */
bool cst816::dataAvailable()
{
    bool available = this->available;
    this->available = false;
    return available;
}

/**
 * @public
 * @brief Get the X position of the touch
 * @returns X position
 */
uint32_t cst816::getX()
{
    return this->report.x;
}

/**
 * @public
 * @brief Get the Y position of the touch
 * @returns Y position
 */
uint32_t cst816::getY()
{
    return this->report.y;
}

/**
 * @private
 * @brief Fetch the touch data
 */
void cst816::fetch()
{
    uint8_t data[6] = { 0 };
    this->touchRead(CST816_ADDR, 0x01, data, 6);

    uint16_t x = (uint16_t)(((data[2] & 0x0f) << 8) | data[3]);
    uint16_t y = (uint16_t)(((data[4] & 0x0f) << 8) | data[5]);

    this->report = {
        .gesture = data[0],
        .points = data[1],
        .x = x,
        .y = y
    };
    
    this->available = true;
}

/**
 * @private
 * @brief Get the version of the CST816
 * @returns Version
 */
uint32_t cst816::getVersion()
{
    uint8_t data = 0;
    this->touchRead(CST816_ADDR, 0x15, &data);
    return data;
}

/**
 * @private
 * @brief Get the version of the CST816
 * @param data Version data
 */
void cst816::getVersionValue(uint32_t* data)
{
    uint8_t buf[3] = { 0 };
    this->touchRead(CST816_ADDR, 0xa7, buf, 3);
    *data = (buf[0] << 16) | (buf[1] << 8) | buf[2];
}