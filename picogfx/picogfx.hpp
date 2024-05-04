#pragma once

#include "graphics.hpp"
#include "print.hpp"
#include "gradient.hpp"
#include "gauge.hpp"

class PicoGFX {
public:
    PicoGFX(Print* print, Graphics* graphics, Gradient* gradient) :
        print(print),
        graphics(graphics),
        gradient(gradient) {
        fillLookupTables();
    }

    Print& getPrint();
    Graphics& getGraphics();
    Gradient& getGradient();

    Print* getPrintPtr();
    Graphics* getGraphicsPtr();
    Gradient* getGradientPtr();

private:
    Print* print;
    Graphics* graphics;
    Gradient* gradient;
};