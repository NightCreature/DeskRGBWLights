// LedUtilityFunctions.h
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Color.h"
#include "lib8tion.h"

#include <Adafruit_NeoPixel.h>

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

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
inline void DrawPixels(float position, float amountToDraw, Color color, Adafruit_NeoPixel& ledStrip)
{
    //have to figure out if position is on a pixel boundary or not
//figure out the start fractional
    uint32_t startIndex = static_cast<uint32_t>(position);
    float remainingPixels = position - startIndex; //this will give us the fractional part of start pos

    if (remainingPixels > 0.0f)
    {
        //need to set a fractional color value here

        ledStrip.setPixelColor(startIndex, ColorFraction(color, remainingPixels).m_wrgb);
        startIndex++; //add one because we always start one further
    }

    remainingPixels = min(amountToDraw, ledStrip.numPixels() - position); //init this with the rest of the pixels we should color;
    while (remainingPixels > 1.0) //While we have whole pixels to color lets do that
    {
        ledStrip.setPixelColor(startIndex, color.m_wrgb);
        ++startIndex;
        remainingPixels -= 1.0f;
    }

    if (remainingPixels > 0.0f)//we have some left over color we need to color
    {
        ledStrip.setPixelColor(startIndex, ColorFraction(color, remainingPixels).m_wrgb);
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
inline void fadeToBlackBy(Adafruit_NeoPixel& ledStrip, uint8_t fadeBy)
{
    uint8_t fadeFactor = 255 - fadeBy;
    uint8_t* pixels = ledStrip.getPixels();
    for (uint16_t counter = 0; counter < ledStrip.numPixels() * 4; counter += 4)
    {
        nscale8x3(pixels[counter], pixels[counter + 1], pixels[counter + 2], fadeFactor);
        uint8_t tempValue = 255;
        nscale8x3(pixels[counter + 3], tempValue, tempValue, fadeFactor);
    }
}