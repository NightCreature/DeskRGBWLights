// LedUtilityFunctions.h
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Color.h"
#include "lib8tion.h"

inline Color ColorFraction(Color color, float fraction)
{
    //Reduces color intensity by the fraction
    fraction = min(1.0f, fraction);//Make sure we are not going over the brighness
    uint8_t fractional = static_cast<uint8_t>(255.0f * fraction);
    Color retVal;
    retVal.m_w = (color.m_w * fractional) >> 8; //Divid by 256 this is faster than a diff
    retVal.m_r = (color.m_r * fractional) >> 8;
    retVal.m_g = (color.m_g * fractional) >> 8;
    retVal.m_b = (color.m_b * fractional) >> 8;

    return retVal;
}


//C version
inline void DrawPixels(float position, float amountToDraw, Color color, Color* colors, uint32_t n)
{
    //have to figure out if position is on a pixel boundary or not
    //figure out the start fractional
    uint32_t startIndex = static_cast<uint32_t>(position);
    float remainingPixels = position - startIndex; //this will give us the fractional part of start pos

    if (remainingPixels > 0.0f)
    {
        //need to set a fractional color value here
        colors[startIndex] = ColorFraction(color, remainingPixels);
        startIndex++; //add one because we always start one further
    }

    remainingPixels = min(amountToDraw, n - position); //init this with the rest of the pixels we should color;
    while (remainingPixels > 1.0) //While we have whole pixels to color lets do that
    {
        colors[startIndex] = color;
        ++startIndex;
        remainingPixels -= 1.0f;
    }

    if (remainingPixels > 0.0f)//we have some left over color we need to color
    {
        colors[startIndex] = ColorFraction(color, remainingPixels);
    }
}

//C++ array length safe call
template< uint32_t n>
inline void DrawPixels(float position, float amountToDraw, Color color, Color(&colors)[n])
{
    DrawPixels(position, amountToDraw, color, &color, n);
}

//Working in bytes here
inline void fadeToBlackBy(uint8_t* leds, uint16_t numLeds, uint8_t fadeBy)
{
    uint8_t fadeFactor = 255 - fadeBy;
    for (uint16_t counter = 0; counter < numLeds * 4; counter += 4)
    {
        nscale8x3(leds[counter], leds[counter + 1], leds[counter + 2], fadeFactor);
        uint8_t tempValue = 255;
        nscale8x3(leds[counter + 3], tempValue, tempValue, fadeFactor);
    }
}
