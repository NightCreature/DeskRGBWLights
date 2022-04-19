#include "TwinkleEffect.h"

#include "LedUtilityFunctions.h"

#include <Adafruit_NeoPixel.h>


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void BaseTwinkle::Initialise()
{
    m_ledStrip.clear();
    m_timeAccumulator = 0.0f;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void TwinkleEffect::Update(float elapsedTime)
{
    fadeToBlackBy(m_ledStrip, 8);

    if (HasTimeDelayElapsed(elapsedTime, 0.25)) //After 1/4 second do the effect and reset the acc
    {
        m_ledStrip.setPixelColor(random(m_ledStrip.numPixels()), Adafruit_NeoPixel::ColorHSV(0 + (random(255) * 255 * 65536) / m_ledStrip.numPixels(), 255, 255));
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void XMasTwinkle::Update(float elapsedTime)
{
    fadeToBlackBy(m_ledStrip, 8);
    if (HasTimeDelayElapsed(elapsedTime, 0.25))
    {
        uint32_t color;
        if (random(2))
        {
            color = m_ledStrip.Color(255, 0, 0, 0);
        }
        else
        {
            color = m_ledStrip.Color(0, 255, 0, 0);
        }
        m_ledStrip.setPixelColor(random(m_ledStrip.numPixels()), color);
    }
}
