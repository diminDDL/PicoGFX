#include "Print.hpp"

/**
 * @brief Construct a new Print object
 * @param display Display to print on
*/
Print::Print(Display* display)
{
    this->display = display;
}

#pragma region Write char
/**
 * @brief Write a character on the display
 * @param c Character to print
 * @param size Size of the character
*/
void Print::write(char c, uchar size, uchar base)
{
    this->print((long)c, this->display->getFillColor().getOppositeColor(), size, base);
}

/**
 * @brief Write a character on the display
 * @param c Character to print
 * @param color Character color
 * @param size Size of the character
*/
void Print::print(char c, Color color, uchar size, uchar base)
{
    this->print((long)c, color, size, base);
}
#pragma endregion

#pragma region Write uchar
/**
 * @brief Write a number on the display
 * @param number Number to print
 * @param size Size of the number
*/
void Print::print(uchar c, uchar size, uchar base)
{
    this->print((ulong)c, this->display->getFillColor().getOppositeColor(), size, base);
}

/**
 * @brief Write a number on the display
 * @param number Number to print
 * @param color Number color
 * @param size Size of the number
*/
void Print::print(uchar c, Color color, uchar size, uchar base)
{
    this->print((ulong)c, color, size, base);
}
#pragma endregion

#pragma region Write int
/**
 * @brief Write a number on the display
 * @param number Number to print
 * @param size Size of the number
*/
void Print::print(int number, uchar size, uchar base)
{
    this->print((long)number, this->display->getFillColor().getOppositeColor(), size, base);
}

/**
 * @brief Write a number on the display
 * @param number Number to print
 * @param color Number color
 * @param size Size of the number
*/
void Print::print(int number, Color color, uchar size, uchar base)
{
    this->print((long)number, color, size, base);
}
#pragma endregion

#pragma region Write uint
/**
 * @brief Write a number on the display
 * @param number Number to print
 * @param size Size of the number
*/
void Print::print(uint number, uchar size, uchar base)
{
    this->print((ulong)number, this->display->getFillColor().getOppositeColor(), size, base);
}

/**
 * @brief Write a number on the display
 * @param number Number to print
 * @param color Number color
 * @param size Size of the number
*/
void Print::print(uint number, Color color, uchar size, uchar base)
{
    this->print((ulong)number, color, size, base);
}
#pragma endregion

#pragma region Write long
/**
 * @brief Write a number on the display
 * @param number Number to print
 * @param size Size of the number
*/
void Print::print(long number, uchar size, uchar base)
{
    this->print(number, this->display->getFillColor().getOppositeColor(), size, base);
}

/**
 * @brief Write a number on the display
 * @param number Number to print
 * @param color Number color
 * @param size Size of the number
*/
void Print::print(long number, Color color, uchar size, uchar base)
{
    // set base to decimal 
    // convert the number to a string
    char buffer[CHARACTER_BUFFER_SIZE];
    itoa(number, buffer, base);
    // write the string
    this->print(buffer, color, size);
}
#pragma endregion

#pragma region Write ulong
/**
 * @brief Write a number on the display
 * @param number Number to print
 * @param size Size of the number
*/
void Print::print(ulong number, uchar size, uchar base)
{
    this->print(number, this->display->getFillColor().getOppositeColor(), size, base);
}

/**
 * @brief Write a number on the display
 * @param number Number to print
 * @param color Number color
 * @param size Size of the number
*/
void Print::print(ulong number, Color color, uchar size, uchar base)
{
    // convert the number to a string
    char buffer[CHARACTER_BUFFER_SIZE];    // largest number a long can represent is 9 223 372 036 854 775 807
    itoa(number, buffer, base);
    // write the string
    this->print(buffer, color, size);
}
#pragma endregion

#pragma region Write float
/**
 * @brief Write a number on the display
 * @param number Number to print
 * @param precision Number of decimal places to print
 * @param size Size of the number
*/
void Print::print(double number, uint precision, uchar size)
{
    this->print(number, this->display->getFillColor().getOppositeColor(), precision, size);
}

/**
 * @brief Write a number on the display
 * @param number Number to print
 * @param color Number color
 * @param precision Number of decimal places to print
 * @param size Size of the number
*/
void Print::print(double number, Color color, uint precision, uchar size)
{
    // convert the number to a string
    char buffer[CHARACTER_BUFFER_SIZE] = {0};    // largest number a double can represent is 1.79769e+308
    this->floatToString(number, buffer, precision);
    // write the string
    this->print(buffer, color, size);
}
#pragma endregion

#pragma region Write string
/**
 * @brief Write a character on the display
 * @param character Character to print
 * @param size Size of the character
*/
void Print::print(const char* text, uchar size)
{
    this->print(text, this->display->getFillColor().getOppositeColor(), size);
}

/**
 * @brief Write a character on the display
 * @param character Character to print
 * @param color Character color
 * @param size Size of the character
*/
void Print::print(const char* text, Color color, uchar size)
{
    // copy the Point to local variables
    Point location = this->display->getCursor();
    // copy the Point to local variables
    uint x = location.X();
    uint y = location.Y();
    // get the length of the text
    uint length = strlen(text);

    // loop through the text
    for(int i = 0; i < length; i++)
    {
        // if the text is a new line, move the text to the next line
        if (text[i] == '\n')
        {
            x = 0;
            y += FONT_HEIGHT * size;
            continue;
        }
        // if the text is a tab move the text to the next tab stop
        else if (text[i] == '\t')
        {
            x += FONT_WIDTH * size * TAB_SIZE;
            continue;
        }
        // check if the text is going to go off the screen by checking the future x Point with the width of the screen
        else if ((x + FONT_WIDTH * size) > this->display->getWidth())
        {
            // move the text to the next line
            x = 0;
            y += FONT_HEIGHT * size;
        }
        // if we overflowed the screen, begin from the top again
        if ((y + FONT_HEIGHT * size) > this->display->getHeight())
        {
            y = 0;
        }

        // increment the Point
        x += this->drawAscii(text[i], {x, y}, size, color);
    }

    // set the cursor
    this->display->setCursor(Point(x, y));
}

/**
 * @brief Write a character on the display
 * @param value Boolean to print
*/
void Print::print(bool value, uchar size)
{
    this->print(value ? "true" : "false", size);
}
#pragma endregion

#pragma region Print char
/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param size Size of the number
*/
void Print::println(char c, uchar size, uchar base)
{
    this->println((long)c, this->display->getFillColor().getOppositeColor(), size);
}

/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param color Number color
 * @param size Size of the number
*/
void Print::println(char c, Color color, uchar size, uchar base)
{
    this->println((long)c, color, size);
}
#pragma endregion

#pragma region Print uchar
/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param size Size of the number
*/
void Print::println(uchar number, uchar size, uchar base)
{
    this->println((long)number, this->display->getFillColor().getOppositeColor(), size, base);
}

/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param color Number color
 * @param size Size of the number
*/
void Print::println(uchar number, Color color, uchar size, uchar base)
{
    this->println((long)number, color, size, base);
}
#pragma endregion

#pragma region Print int
/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param size Size of the number
*/
void Print::println(int number, uchar size, uchar base)
{
    this->println((long)number, this->display->getFillColor().getOppositeColor(), size, base);
}

/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param color Number color
 * @param size Size of the number
*/
void Print::println(int number, Color color, uchar size, uchar base)
{
    this->println((long)number, color, size, base);
}
#pragma endregion

#pragma region Print uint
/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param size Size of the number
*/
void Print::println(uint number, uchar size, uchar base)
{
    this->println((ulong)number, this->display->getFillColor().getOppositeColor(), size, base);
}

/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param color Number color
 * @param size Size of the number
*/
void Print::println(uint number, Color color, uchar size, uchar base)
{
    this->println((ulong)number, color, size, base);
}
#pragma endregion

#pragma region Print long
/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param size Size of the number
*/
void Print::println(long number, uchar size, uchar base)
{
    this->println(number, this->display->getFillColor().getOppositeColor(), size, base);
}

/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param color Number color
 * @param size Size of the number
*/
void Print::println(long number, Color color, uchar size, uchar base)
{
    // convert the number to a string
    char buffer[CHARACTER_BUFFER_SIZE];    // largest number a long can represent is 9 223 372 036 854 775 807
    itoa(number, buffer, base);
    // write the string
    this->println(buffer, color, size);
}
#pragma endregion

#pragma region Print ulong
/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param size Size of the number
*/
void Print::println(ulong number, uchar size, uchar base)
{
    this->println(number, this->display->getFillColor().getOppositeColor(), size, base);
}

/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param color Number color
 * @param size Size of the number
*/
void Print::println(ulong number, Color color, uchar size, uchar base)
{
    // convert the number to a string
    char buffer[CHARACTER_BUFFER_SIZE];    // largest number a long can represent is 9 223 372 036 854 775 807
    itoa(number, buffer, base);
    // write the string
    this->println(buffer, color, size);
}
#pragma endregion

#pragma region Print float
/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param precision Number of decimal places to print
 * @param size Size of the number
*/
void Print::println(double number, uint precision, uchar size)
{
    this->println(number, this->display->getFillColor().getOppositeColor(), precision, size);
}

/**
 * @brief Print a number on the display
 * @param number Number to print
 * @param color Number color
 * @param precision Number of decimal places to print
 * @param size Size of the number
*/
void Print::println(double number, Color color, uint precision, uchar size)
{
    this->print(number, color, precision, size);
    this->print("\n");
}
#pragma endregion

#pragma region Print string
/**
 * @brief Print a character on the display
 * @param character Character to print
 * @param size Size of the character
*/
void Print::println(const char* text, uchar size)
{
    this->println(text, this->display->getFillColor().getOppositeColor(), size);
}

/**
 * @brief Print a character on the display
 * @param character Character to print
 * @param color Character color
 * @param size Size of the character
*/
void Print::println(const char* text, Color color, uchar size)
{
    this->print(text, color, size);
    this->print("\n", color, size);
}

/**
 * @brief Write a character on the display
 * @param value Boolean to print
*/
void Print::println(bool value, uchar size)
{
    this->println(value ? TRUE : FALSE, size);
}
#pragma endregion

#pragma region Print void
/**
 * @brief Print a newline
*/
void Print::println(void)
{
    this->println("\n");
}
#pragma endregion

#pragma region Get length of string
/**
 * @brief Get the length of a string
 * @param num String to get the length of
 * @param size Size of the string
 * @param base Base of the number
 * @return Length of the string
*/
uint Print::getStringLength(char num, uchar size, uchar base)
{
    return this->getStringLength((long)num, size, base);
}

/**
 * @brief Get the length of a string
 * @param num String to get the length of
 * @param size Size of the string
 * @param base Base of the number
 * @return Length of the string
*/
uint Print::getStringLength(uchar num, uchar size, uchar base)
{
    return this->getStringLength((ulong)num, size, base);
}

/**
 * @brief Get the length of a string
 * @param num String to get the length of
 * @param size Size of the string
 * @param base Base of the number
 * @return Length of the string
*/
uint Print::getStringLength(int num, uchar size, uchar base)
{
    return this->getStringLength((long)num, size, base);
}

/**
 * @brief Get the length of a string
 * @param num String to get the length of
 * @param size Size of the string
 * @param base Base of the number
 * @return Length of the string
*/
uint Print::getStringLength(uint num, uchar size, uchar base)
{
    return this->getStringLength((ulong)num, size, base);
}

/**
 * @brief Get the length of a string
 * @param num String to get the length of
 * @param size Size of the string
 * @param base Base of the number
 * @return Length of the string
*/
uint Print::getStringLength(short num, uchar size, uchar base)
{
    return this->getStringLength((long)num, size, base);
}

/**
 * @brief Get the length of a string
 * @param num String to get the length of
 * @param size Size of the string
 * @param base Base of the number
 * @return Length of the string
*/
uint Print::getStringLength(ushort num, uchar size, uchar base)
{
    return this->getStringLength((ulong)num, size, base);
}

/**
 * @brief Get the length of a string
 * @param num String to get the length of
 * @param size Size of the string
 * @param base Base of the number
 * @return Length of the string
*/
uint Print::getStringLength(long num, uchar size, uchar base)
{
    // convert the number to a string
    char buffer[CHARACTER_BUFFER_SIZE];    // largest number a long can represent is 9 223 372 036 854 775 807
    itoa(num, buffer, base);
    uint len = strlen(buffer);
    return (len * size * FONT_WIDTH);
}

/**
 * @brief Get the length of a string
 * @param num String to get the length of
 * @param size Size of the string
 * @param base Base of the number
 * @return Length of the string
*/
uint Print::getStringLength(ulong num, uchar size, uchar base)
{
    // convert the number to a string
    char buffer[CHARACTER_BUFFER_SIZE];    // largest number a long can represent is 9 223 372 036 854 775 807
    itoa(num, buffer, base);
    uint len = strlen(buffer);
    return (len * size * FONT_WIDTH);
}

/**
 * @brief Get the length of a string
 * @param num String to get the length of
 * @param precision Number of decimal places to print
 * @param size Size of the string
 * @param base Base of the number
 * @return Length of the string
*/
uint Print::getStringLength(double num, uchar precision, uchar size)
{
    // convert the number to a string
    char buffer[CHARACTER_BUFFER_SIZE];    // largest number a long can represent is 9 223 372 036 854 775 807
    floatToString(num, buffer, precision);
    uint len = strlen(buffer);
    return (len * size * FONT_WIDTH);
}

/**
 * @brief Get the length of a string
 * @param text String to get the length of
 * @param size Size of the string
 * @return Length of the string
*/
uint Print::getStringLength(const char* text, uchar size)
{
    return (strlen(text) * size * FONT_WIDTH);
}

/**
 * @brief Get the length of a string
 * @param value String to get the length of
 * @param size Size of the string
 * @return Length of the string
*/
uint Print::getStringLength(bool value, uchar size)
{
    if(value)
        return (strlen(TRUE) * size * FONT_WIDTH);
    else
        return (strlen(FALSE) * size * FONT_WIDTH);
}
#pragma endregion

#pragma region Convert float to string
/**
 * @private
 * @brief Convert float to string
 * @param value Value to convert
 * @param buffer Buffer to write to
 * @param precision Precision of the value
*/
void Print::floatToString(double num, char* buffer, uint precision)
{
    // if precision is 0, just return the integer part
    if(precision == 0)
    {
        itoa((long)num, buffer, 10);
        return;
    }

    // print the sign if the number is negative
	if(num < 0.0)
	{
		*buffer++ = '-';
		num = -num;
	}

	// round the number to the precision
	double rounding = 0.5;
	for(uchar i = 0; i < (precision + 1); ++i)
		rounding /= 10.0;
	num += rounding;

	// print the integer part
	unsigned long integer = (unsigned long)num;
	double remainder = num - (double)integer;

    // store the number of integers for fast reversing later
    uint integers = 0;
    // loop until the integer is 0 at least once
    do
    {
        // add the first digit to the buffer
        *buffer++ = '0' + (integer % 10);
        integer /= 10;
        integers++;
    } while(integer > 0);

    // reverse the buffer to get the correct order
    this->reverse(buffer - integers, integers);

	// print the decimal point
	if(precision > 0)
		*buffer++ = '.';

	// print the decimal part
	while(precision-- > 0)
	{
		remainder *= 10.0;
		int digit = int(remainder);
        *buffer++ = '0' + digit;
		remainder -= digit;
        integers++;
	}
}

void Print::reverse(char* buffer, uint length)
{
    for(int i = 0; i < length/2; i++) {
        char temp = *(buffer + i);
        *(buffer + i) = *(buffer + length - i - 1);
        *(buffer + length - i - 1) = temp;
    }
}
#pragma endregion

#pragma region Write Ascii to display
/**
 * @private
 * @brief Draw an ascii character on the display
 * @param character Character to draw
 * @param Point Point to draw at
 * @param size Text size
 * @param color Color to draw the character
 * @return Width of the character
*/
uint Print::drawAscii(const char character, Point point, uint size, Color color)
{
    // get the relevant bitmap data which is indexed according to the ascii table
    const uint* bitmap = FONT(character);

    // if the bitmap is a null pointer, return
    if (bitmap == nullptr)
        return 0;

    // check if size is 0
    if (size == 0)
        size = 1;

    // make sure the font size will not overflow the buffer
    if ((FONT_WIDTH * FONT_HEIGHT) * size > this->display->getBufferSize())
        return 0;

    uint rowPosition = 0;
    uint columnPosition = 0;
    uint rowSize = FONT_WIDTH * size;

    for (int j = 0; j < FONT_DATA; j++)
    {
        // get the current data
        uint data = bitmap[j] * size;

        // if the current data is 0, we have completed our loop
        if (data == 0)
            break;

        for (int i = 0; i < data; i++)
        {
            if (j & 0x1)
            {
                for (uint k = 0; k < size; k++)
                {
                    Point pixelPos(point.x + rowPosition, point.y + columnPosition + k);
                    this->display->setPixel(pixelPos, color);
                }
            }
            else if (size > 1)
            {
                for (uint k = 0; k < size; k++)
                {
                    Point pixelPos(point.x + rowPosition, point.y + columnPosition + k);
                    Color prevColor = this->display->getPixel(pixelPos);
                    this->display->setPixel(pixelPos, prevColor);
                }
            }

            rowPosition++;

            if (rowPosition == rowSize)
            {
                // reset the row position
                rowPosition = 0;

                // copy the column to the buffer as many times as specified by the size
                for (int j = 0; j < size; j++)
                {
                    // copy the column to the buffer
                    for (uint k = 0; k < rowSize; k++)
                    {
                        Point pixelPos(point.x + k, point.y + columnPosition);
                        Color pixelColor = this->display->getPixel(pixelPos);
                        this->display->setPixel(pixelPos, pixelColor);
                    }
                    columnPosition++;
                }
            }
        }
    }

    // return the character width
    return FONT_WIDTH * size;
}
#pragma endregion