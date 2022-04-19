#pragma once

#include "arduino.h"
#include "Effect.h"


class FireEffect : public Effect
{
public:
    FireEffect(Adafruit_NeoPixel& strip, int numberOfSpots):
        Effect("Fire", strip),
        m_currentNumberOfFires(min(numberOfSpots, maxNumberOfFires))
    {}

    void Update(float elapsedTime);
private:
    struct FireEffectState
    {

    };

    const static int maxNumberOfFires = 3; //Edges plus middle
    FireEffectState m_fires[maxNumberOfFires];
    int m_currentNumberOfFires;
};

