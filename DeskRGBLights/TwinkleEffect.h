#pragma once

#include "Effect.h"

class BaseTwinkle : public Effect
{
public:
    BaseTwinkle(const char* name, Adafruit_NeoPixel& strip) : Effect(name, strip) {}
    void Initialise() override;
};

class TwinkleEffect : public BaseTwinkle
{
public:
    TwinkleEffect(Adafruit_NeoPixel& strip) : BaseTwinkle("Twinkle", strip) {}

    void Update(float elapsedTime) override;
};

class XMasTwinkle : public BaseTwinkle
{
public:
    XMasTwinkle(Adafruit_NeoPixel& strip) : BaseTwinkle("XMasTwinkle", strip) {}

    void Update(float elapsedTime) override;

};