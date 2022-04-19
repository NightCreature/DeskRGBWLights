#pragma once
#include "Effect.h"

class KnightRider : public Effect
{
public:
    KnightRider(Adafruit_NeoPixel& strip, int8_t length, uint32_t color) : 
        Effect("KnightRider", strip),
        m_color(color),
        m_length(length)
    {
        m_moveDirection = 1;
        m_position = 0.3f;
        m_moveAmount = 0.75f;
    }

    void Initialise() override;
    void Update(float elapsedTime) override;

private:
    float m_position = 0.3f;
    float m_moveAmount = 0.75f;
    uint32_t m_color;
    uint8_t m_length;
    char m_moveDirection = 1; //start of moving to the right
};

