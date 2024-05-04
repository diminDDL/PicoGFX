#pragma once

#include "graphics.hpp"
#include "print.hpp"
#include "gradient.hpp"
#include "encoder.h"
#include "gauge.hpp"

class PicoGFX {
public:
    PicoGFX(Print* print, Graphics* graphics, Gradient* gradient, Encoder* encoder) :
        print(print),
        graphics(graphics),
        gradient(gradient) {
        fillLookupTables();
    }

    Print& getPrint();
    Graphics& getGraphics();
    Gradient& getGradient();
    Encoder& getEncoder();

    Print* getPrintPtr();
    Graphics* getGraphicsPtr();
    Gradient* getGradientPtr();
    Encoder* getEncoderPtr();

private:
    Print* print;
    Graphics* graphics;
    Gradient* gradient;
    Encoder* encoder;
};