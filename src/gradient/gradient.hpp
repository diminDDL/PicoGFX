#pragma once

#include "color.h"
#include "structs.h"
#include "shapes.h"
#include "gfxmath.h"

class Gradient
{
public:
    Gradient(unsigned short* frameBuffer, display_config_t* config);

    void fillGradient(Color startColor, Color endColor, Point start, Point end);
    void drawRotCircleGradient(Point center, int radius, int rotationSpeed, Color start, Color end);
    void drawRotRectGradient(Point center, int width, int height, int rotationSpeed, Color start, Color end);
private:
    unsigned short* frameBuffer;
    display_config_t* config;
    size_t totalPixels;

    unsigned int theta; // The angle of the rotating gradient
    const int firstQuadrant = NUMBER_OF_ANGLES / 4;
    const int secondQuadrant = NUMBER_OF_ANGLES / 2;
    const int thirdQuadrant = NUMBER_OF_ANGLES * 3 / 4;
};