#include "KnightRider.h"
#include "LedUtilityFunctions.h"

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void KnightRider::Initialise()
{
    m_moveDirection = 1;
    m_position = 0.3f;
    m_moveAmount = 0.75f;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void KnightRider::Update(float elapsedTime)
{
    if (HasTimeDelayElapsed(elapsedTime, 0.05))
    {
        m_ledStrip.clear();

        //Either we are at the start or end of the strip so invert direction
        if (m_position < 0.2f || m_position > m_ledStrip.numPixels())
        {
            m_moveDirection = m_moveDirection * -1; //Invert direction;
            m_position = m_position < 0.2f ? 0.3f : 118.8f; //return to the beginning of the strip
        }
        else
        {
            DrawPixels(m_position, m_length, m_color, m_ledStrip);
            m_position = m_position + (static_cast<int>(m_length) * m_moveDirection * m_moveAmount);
        }
    }
}
