#pragma once
#include <stdint.h>

//Uinion that really acts like a uint32 but allows us to access the components more easily, this makes 
//Neopixel expects W << 24, R << 16, B << 8, G for the byte positions
//This is based on the FASTLed CRGB structure
struct Color
{
    union
    {
        //Helper so we can access the component channels
        struct
        {
            union
            {
                uint8_t m_w;
                uint8_t m_white;
            };
            union
            {
                uint8_t m_r;
                uint8_t m_red;
            };
            union
            {
                uint8_t m_g;
                uint8_t m_green;
            };
            union
            {
                uint8_t m_b;
                uint8_t m_blue;
            };
        };
        //full access for setPixel
        uint32_t m_wrgb;
        //Raw array access
        uint8_t m_bytes[4];
    };
};