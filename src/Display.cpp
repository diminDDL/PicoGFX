#include <Display.hpp>

/**
 * @brief Display initialization
 * @param spi SPI bus
 * @param pins Pins
 * @param params Display parameters
 * @param dimming Enable dimming
 * @param backlight Enable backlight
*/
Display::Display(HardwareSPI* spi, Display_Pins* pins, Display_Params* params)
{
    this->spi = spi;
    this->pins = pins;
    this->params = params;
    this->type = params->type;
    this->totalPixels = params->width * params->height;

    // init the rest of the pins
    gpio_init(this->pins->rst);
    // set the rest of the pins to GPIO output
    gpio_set_dir(this->pins->rst, GPIO_OUT);
    // set the pins to high
    gpio_put(this->pins->rst, 1);

    // set up the backlight pin depending on the dimming setting
    this->backlight = this->pins->bl != -1;
    if (this->backlight)
    {
        if (params->dimming)
        {
            // enable dimming
            this->dimmingEnabled = true;
            gpio_set_function(this->pins->bl, GPIO_FUNC_PWM);
            // get the PWM slice number
            uint sliceNum = pwm_gpio_to_slice_num(this->pins->bl);
            this->sliceNum = sliceNum;
            // get the PWM channel
            uint chan = pwm_gpio_to_channel(this->pins->bl);
            this->pwmChannel = chan;
            // turn on the PWM slice
            pwm_set_enabled(sliceNum, true);
            // set the PWM wrap value
            pwm_set_wrap(sliceNum, 255);
            // set the PWM value
            pwm_set_chan_level(sliceNum, chan, 127);
        }
        else
        {
            this->dimmingEnabled = false;
            gpio_init(this->pins->bl);
            gpio_set_dir(this->pins->bl, GPIO_OUT);
            gpio_put(this->pins->bl, 1);
        }
    }
}

/**
 * @brief Display initialization
*/
void Display::init()
{
    if (this->type == display_type_t::GC9A01)
        this->GC9A01_Init();
    else if (this->type == display_type_t::ST7789)
        this->ST7789_Init();

    // clear the display
    this->clear();
    // turn on the display
    this->writeData(DISPON, (const uint8_t*)NULL, 0);
}

/**
 * @brief Clear the display by drawing a black rectangle
*/
void Display::clear()
{
    // set the cursor position to the top left
    this->setCursor({ 0, 0 });
    // fill the frame buffer
    for (int i = 0; i < this->totalPixels; i++)
        this->frameBuffer[i] = 0x0000;
    this->setCursor({ 0, 0 });
    this->update();
}

/**
 * @brief Print the frame buffer to the display
*/
void Display::update(bool framecounter)
{
    this->setCursor({ 0, 0 });
    this->writePixels(this->frameBuffer, this->totalPixels);
    if (!framecounter) return;

    this->framecounter++;
    if ((time_us_64() - this->timer) >= 1000000)
    {
        this->timer = time_us_64();
        this->frames = this->framecounter;
        this->framecounter = 0;
    }
}

/**
 * @brief Put a pixel in the framebuffer
 * @param Point Points to draw the pixel at
 * @param color Color to draw in
*/
void Display::setPixel(Point point, Color color)
{
    // set the framebuffer pixel
    this->frameBuffer[point.x + point.y * this->params->width] = color.to16bit();
}

/**
 * @brief Put a pixel in the framebuffer
 * @param Point Buffer index
 * @param color Color to draw in as a 16 bit value
*/
void Display::setPixel(uint index, ushort color)
{
    // set the framebuffer pixel
    this->frameBuffer[index] = color;
}

/**
 * @brief Get a pixel from the framebuffer
 * @param Point Point to get the pixel from
 * @return Color The color of the pixel
*/
Color Display::getPixel(Point point)
{
    return Color(this->frameBuffer[point.x + point.y * this->params->width]);
}

/**
 * @brief Get a pixel from the framebuffer
 * @param Point Buffer index
 * @return Color The color of the pixel
*/
ushort Display::getPixel(uint index)
{
    return this->frameBuffer[index];
}

/**
 * @brief Get the size of the display framebuffer
 * @return Size of the framebuffer
*/
size_t Display::getBufferSize()
{
    return FRAMEBUFFER_SIZE;
}

/**
 * @brief Turn the display on
*/
void Display::displayOn()
{
    this->writeData(Display_Commands::DISPON);
    sleep_ms(10);
}

/**
 * @brief Turn the display off
*/
void Display::displayOff()
{
    this->writeData(Display_Commands::DISPOFF);
    sleep_ms(10);
}

/**
 * @brief Set the cursor position
 * @param Point Point to set the cursor to
*/
void Display::setCursor(Point point)
{
    // set the pixel x address
    this->columnAddressSet(
        point.x + this->params->columnOffset1,
        (this->params->width - 1) + this->params->columnOffset2
    );
    // set the pixel y address
    this->rowAddressSet(
        point.y + this->params->rowOffset1,
        (this->params->height - 1) + this->params->rowOffset2
    );
    // set the internal cursor position
    this->cursor = point;
}

/**
 * @brief Get the cursor position
 * @return Point The cursor position
*/
Point Display::getCursor()
{
    return this->cursor;
}

/**
 * @brief Get the center of the display
 * @return Point The center of the display
*/
Point Display::getCenter()
{
    Point Point = {
        this->params->width / 2,
        this->params->height / 2
    };
    return Point;
}

/**
 * @brief Rotate the display
 * @param rotation Rotation to set
*/
void Display::setRotation(int rotation)
{
    // save the rotation
    this->params->rotation = (int)rotation;
    unsigned int width = this->params->width;
	unsigned int height = this->params->height;
	unsigned int maxHeight = this->maxHeight;
	unsigned int maxWidth = this->maxWidth;

    switch(rotation)
    {
		case 0:
			this->params->height = height;
			this->params->width = width;
			this->maxHeight = maxHeight;
			this->maxWidth = maxWidth;
            break;
        case 1:
			this->params->height = width;
			this->params->width = height;
			this->maxHeight = maxWidth;
			this->maxWidth = maxHeight;
            break;
        case 2:
			this->params->height = height;
			this->params->width = width;
			this->maxHeight = maxHeight;
			this->maxWidth = maxWidth;
            break;
        case 3:
			this->params->height = width;
			this->params->width = height;
			this->maxHeight = maxWidth;
			this->maxWidth = maxHeight;
            break;
    }

    // set the rotation based on the display type
    if (this->type == display_type_t::GC9A01)
        this->GC9A01_SetRotation(rotation);
    else if (this->type == display_type_t::ST7789)
        this->ST7789_SetRotation(rotation);
}

/**
 * @brief Set the backlight brightness
 * @param brightness Brightness (0-255) if dimming is enabled, brightness (0-1) if dimming is disabled
*/
void Display::setBrightness(unsigned char brightness)
{
    if (!this->backlight)
        return;

    // check if dimming is enabled
    if (this->dimmingEnabled)
    {
        // set the brightness
        pwm_set_chan_level(this->sliceNum, this->pwmChannel, brightness);
    }
    else
    {
        // make sure the brightness is between 0 and 1
        brightness = brightness & 0x01;
        // toggle the backlight pin based on the brightness
        gpio_put(this->pins->bl, brightness);
    }
}


/**
 * @private
 * @brief Write data to the display
 * @param command Command to send
 * @param data Data to send
 * @param length Length of the data
*/
void Display::writeData(unsigned char command, const unsigned char* data, size_t length)
{
    // set the data mode
    this->dataMode = false;
    // write the command
    this->spi->spi_write_data(command, data, length);
}

/**
 * @private
 * @brief Set the column address
 * @param x0 Start column
 * @param x1 End column
*/
inline void Display::columnAddressSet(uint x0, uint x1)
{
    // deny out of bounds
    if (x0 >= x1 || x1 >= this->maxWidth)
        return;

    // pack the data
    unsigned char data[4] = {
        (unsigned char)(x0 >> 8),
        (unsigned char)(x0 & 0xff),
        (unsigned char)(x1 >> 8),
        (unsigned char)(x1 & 0xff)
    };

    // write the data
    this->writeData(Display_Commands::CASET, data, sizeof(data));
}

/**
 * @private
 * @brief Set the row address
 * @param y0 Start row
 * @param y1 End row
*/
inline void Display::rowAddressSet(uint y0, uint y1)
{
    // deny out of bounds
    if (y0 >= y1 || y1 >= this->maxHeight)
        return;

    // pack the data
    unsigned char data[4] = {
        (unsigned char)(y0 >> 8),
        (unsigned char)(y0 & 0xff),
        (unsigned char)(y1 >> 8),
        (unsigned char)(y1 & 0xff)
    };

    // write the data
    this->writeData(Display_Commands::RASET, data, sizeof(data));
}

/**
 * @private
 * @brief Write pixels to the display
 * @param data data to write
 * @param length Length of the data
 * @note length should be number of 16 bit pixels, not bytes!
*/
void Display::writePixels(const unsigned short* data, size_t length)
{
    // check if the data mode is set
    if (!this->dataMode)
    {
        // set the data mode
        this->spi->spi_set_data_mode(Display_Commands::RAMWR);
        this->dataMode = true;
    }
    // write the pixels
    this->spi->spi_write_pixels(data, length);
}