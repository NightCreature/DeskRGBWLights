#pragma once
#include <Adafruit_NeoPixel.h>

struct FireWorkState
{
    FireWorkState() :
        m_lifeTime(10),
        m_leftPos(0.0f),
        m_rightPos(0.0f),
        m_trailLength(1.5f),
        m_stepAmount(2.5f),
        m_intenistyFade(64),
        m_color(0),
        m_new(false)
    {}

    int m_lifeTime = 10; //Ten updates and this effect disapears
    float m_leftPos = 0.0f; //out shoots
    float m_rightPos = 0.0f;
    float m_trailLength = 1.5f;
    float m_stepAmount = 2.5f;

    int m_intenistyFade = 64;

    uint32_t m_color = 0;
    bool m_new = false;
};

class FireworkEffect
{
public:
    FireworkEffect() = delete;
    FireworkEffect(Adafruit_NeoPixel& ledStrip, int numberOfSimultaneousFireworks) :
        m_ledStrip(ledStrip),
        m_numberOfSimultaneousFireworks(min(numberOfSimultaneousFireworks, maxActiveFireworks)),
        m_currentActiveNumberFireworks(0)
    {
    }


    void Update(); //Call this each loop

    void Reset()
    {
        m_currentActiveNumberFireworks = 0;
    }
private:
    void SortEffectsByLifetime();

    static const int maxActiveFireworks = 5;
    FireWorkState m_activeFireWorks[maxActiveFireworks];

    Adafruit_NeoPixel& m_ledStrip;
    int m_numberOfSimultaneousFireworks = maxActiveFireworks;
    int m_currentActiveNumberFireworks = 0;
};

