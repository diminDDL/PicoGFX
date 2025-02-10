#include "picogfx.hpp"

/**
 * @brief Get the Print object
 * @return Print&
*/
Print& PicoGFX::getPrint()
{
    return *print;
}

/**
 * @brief Get the Graphics object
 * @return Graphics&
*/
Graphics& PicoGFX::getGraphics()
{
    return *graphics;
}

/**
 * @brief Get the Gradient object
 * @return Gradient&
*/
Gradient& PicoGFX::getGradient()
{
    return *gradient;
}


/**
 * @brief Get the Print pointer
 * @return Print*
*/
Print* PicoGFX::getPrintPtr()
{
    return print;
}

/**
 * @brief Get the Graphics pointer
 * @return Graphics*
*/
Graphics* PicoGFX::getGraphicsPtr()
{
    return graphics;
}

/**
 * @brief Get the Gradient pointer
 * @return Gradient*
*/

Gradient* PicoGFX::getGradientPtr()
{
    return gradient;
}