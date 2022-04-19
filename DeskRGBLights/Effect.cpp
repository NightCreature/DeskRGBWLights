#include "Effect.h"

#include <Adafruit_NeoPixel.h>

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool Effect::HasTimeDelayElapsed(float elapsedTime, float period)
{
    m_timeAccumulator += elapsedTime;
    if (m_timeAccumulator > period)
    {
        m_timeAccumulator = 0.0f;
        return true;
    }

    return false;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void SolidColor::Initialise()
{
    m_ledStrip.clear();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void SolidColor::Update(float elapsedTime)
{
    m_ledStrip.fill(m_color, 0, m_ledStrip.numPixels());
}
