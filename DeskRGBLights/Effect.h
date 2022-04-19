#pragma once

#include <stdint.h>

class Adafruit_NeoPixel;

class Effect
{
public:
    Effect(Adafruit_NeoPixel& strip) : m_name(""), m_ledStrip(strip) {}
    Effect(const char* name, Adafruit_NeoPixel& strip) : m_name(name), m_ledStrip(strip), m_timeAccumulator(0.0f) {}
    virtual ~Effect() {}

    virtual void Initialise() = 0;
    virtual void Update(float elapsedTime) = 0;

    const char* GetEffectName() const { return m_name; }

    bool HasTimeDelayElapsed(float elapsedTime, float period);
protected:
    const char* m_name;
    Adafruit_NeoPixel& m_ledStrip;
    float m_timeAccumulator;
};

class SolidColor : public Effect
{

public:
    SolidColor(const char* name, Adafruit_NeoPixel& strip, uint32_t color) : Effect(name, strip), m_color(color) {}
    void Initialise() override;
    void Update(float elapsedTime) override;
private:
    uint32_t m_color = 0xFFFFFFFF;
};
