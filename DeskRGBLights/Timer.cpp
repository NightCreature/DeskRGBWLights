#include "Timer.h"
#include "arduino.h"

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
Timer::Timer()
{
    m_timeStamp = millis();
    m_elapsedTime = 0.0f;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void Timer::Update()
{
    unsigned long now = millis();
    m_elapsedTime = (now - m_timeStamp) * 0.001f;
    m_timeStamp = now;
}
