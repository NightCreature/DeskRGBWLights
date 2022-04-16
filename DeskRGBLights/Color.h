#pragma once
#include <stdint.h>

//Uinion that really acts like a uint32 but allows us to access the components more easily, this makes 
//Neopixel expects W << 24, R << 16, B << 8, G for the byte positions
//This is based on the FASTLed CRGB structure
struct Color
{
    Color() :
        m_wrgb(0)
    {}

    Color(uint8_t r, uint8_t g, uint8_t b) :
        m_r(r),
        m_g(g),
        m_b(b)
    { }

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) :
        Color(r, g, b)
    { 
        m_w = w;
    }

    Color(uint32_t color) :
        m_wrgb(color) { }

    Color(uint8_t color) :
        Color(color, color, color, color)
    { }

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