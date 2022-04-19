#include "FireworkEffect.h"
#include "LedUtilityFunctions.h"

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void FireworkEffect::Initialise()
{

}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void FireworkEffect::Update(float elpasedTime)
{
    if (HasTimeDelayElapsed(elpasedTime, 0.1))
    {
        //Add one fire work per update when we are not at max yet
        if (m_currentActiveNumberFireworks < m_numberOfSimultaneousFireworks)
        {
            //need to spawn a new firework
            auto& fireWork = m_activeFireWorks[m_currentActiveNumberFireworks];
            fireWork.m_lifeTime = random(5, 10); //Random lifetime
            fireWork.m_trailLength = random(1, 3);

            fireWork.m_intenistyFade = 255 / fireWork.m_lifeTime;//this makes it so that the last update fades the whole effect away
            fireWork.m_new = true;
            //Place effect on strip
            fireWork.m_leftPos = random(0, m_ledStrip.numPixels());
            fireWork.m_rightPos = fireWork.m_leftPos;

            fireWork.m_color = Adafruit_NeoPixel::ColorHSV(0 + (random(255) * 255 * 65536) / m_ledStrip.numPixels(), 255, 255);

            //fireWork.m_color = m_ledStrip.Color(random(128, 255), random(128, 255), random(128, 255), 0); //These are not good colors we are better of getting a hue here this ends up too white

            ++m_currentActiveNumberFireworks;
        }

        //fade LED string
        fadeToBlackBy(m_ledStrip, 64 * 3); //Do a general fade

        for (int counter = 0; counter < m_currentActiveNumberFireworks; ++counter)
        {
            auto& fireWork = m_activeFireWorks[counter];
            if (fireWork.m_lifeTime > 0)
            {
                fireWork.m_leftPos -= fireWork.m_stepAmount;
                fireWork.m_rightPos += fireWork.m_stepAmount;

                fireWork.m_color = fadeToBlackBy(fireWork.m_color, fireWork.m_intenistyFade);
                --fireWork.m_lifeTime;

                if (fireWork.m_new)
                {
                    //Lightup a single pixel and trail off from there
                    DrawPixels(fireWork.m_leftPos, 1, fireWork.m_color, m_ledStrip);
                    fireWork.m_new = false;
                }
                else
                {
                    //This function draws left to write so the right hand trail will have the bright spot wrong
                    DrawPixels(fireWork.m_leftPos, fireWork.m_trailLength, fireWork.m_color, m_ledStrip);
                    DrawPixels(fireWork.m_rightPos + fireWork.m_trailLength, fireWork.m_trailLength, fireWork.m_color, m_ledStrip);
                }
            }
            else
            {
                //This object is dead
            }
        }

        //Need to do some array suffling here to make sure the fireworks that are done are at the back
        //Sort this on lifetime which means the oldest ones are last or first
        SortEffectsByLifetime();
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void FireworkEffect::SortEffectsByLifetime()
{
    int validEffects = 0;
    for (int counter = 0; counter < m_currentActiveNumberFireworks; ++counter)
    {
        auto& fireWork = m_activeFireWorks[counter];
        if (fireWork.m_lifeTime > 0)
        {
            ++validEffects;
        }
        else 
        {
            if (counter < m_currentActiveNumberFireworks - 1)
            {
                //Need to move this one away
                auto& deadFirework = m_activeFireWorks[counter];
                int nextIndex = counter;
                auto& nextFirework = deadFirework;
                while (nextFirework.m_lifeTime <= 0 && nextIndex < m_currentActiveNumberFireworks)
                {
                    nextFirework = m_activeFireWorks[nextIndex];
                    ++nextIndex;
                    
                }

                //found the next valid fire work so move it to this slot
                if (nextIndex < m_currentActiveNumberFireworks)
                {
                    m_activeFireWorks[counter] = nextFirework;
                    ++validEffects;
                }

            }
        }
    }

    m_currentActiveNumberFireworks = validEffects;

    //Modify m_currentActiveNumberFireworks so we have the correct value
}
