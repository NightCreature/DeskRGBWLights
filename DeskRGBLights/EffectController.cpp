#include "EffectController.h"

#include <Arduino.h>
#include "Color.h"
#include "LedUtilityFunctions.h"

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void EffectController::Initialise()
{
    m_ledStrip = Adafruit_NeoPixel(m_numberOfLeds, m_pins.m_ledPin, NEO_GRBW + NEO_KHZ800);

    pinMode(m_pins.m_onOffPin, INPUT);
    pinMode(m_pins.m_selectionPin, INPUT);
    pinMode(m_pins.m_brightnessControllPin, INPUT); //this is an analog input pin

    m_lcd.begin(16, 2);

    pinMode(m_pins.m_lcdContrastPin, OUTPUT);
    analogWrite(m_pins.m_lcdContrastPin, 0); //Set contrast to low some how this is the way we can see this, since this is 10 bit value max would be 1023

    //configure strip options
    m_ledStrip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    m_ledStrip.show();            // Turn OFF all pixels ASAP
    m_ledStrip.setBrightness(m_brightnessValue); // Set BRIGHTNESS to about 1/5 (max = 255)

    DisplayEffectName();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void EffectController::SelectAndShowEffect()
{
    bool needsToCallShow = false;
    m_shouldContinouslyUpdateEffect = false;
    switch (static_cast<Effects>(m_currentEffect->m_index))
    {
    case Off:
    {
        m_ledStrip.clear();
        needsToCallShow = true;
    }
        break;
    case SolidRed:
    {
        ShowSolidColor(m_ledStrip.Color(255, 0, 0, 0));
        needsToCallShow = true;
    }
        break;
    case SolidGreen:
    {
        ShowSolidColor(m_ledStrip.Color(0, 255, 0, 0));
        needsToCallShow = true;
    }
        break;
    case SolidBlue:
    {
        ShowSolidColor(m_ledStrip.Color(0, 0, 255, 0));
        needsToCallShow = true;
    }
        break;
    case SolidWhite:
    {
        ShowSolidColor(m_ledStrip.Color(0, 0, 0, 255));
        needsToCallShow = true;
    }
        break;
    case Rainbow:
    {
        m_ledStrip.rainbow(0, 255, 255, m_brightnessValue, true);
        m_ledStrip.show();
    }
        break;
    case KightRider:
    {
        KnightRider(5);
        m_shouldContinouslyUpdateEffect = true;
        needsToCallShow = true;
    }
        break;
    case Effects::Twinkle:
    {
        Twinkle();
        m_shouldContinouslyUpdateEffect = true;
        needsToCallShow = true;
    }
        break;
    case Count:
    default:
        break;

    }

    if (needsToCallShow)
    {
        m_ledStrip.show();
    }

    DisplayEffectName();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool EffectController::UpdateEffectChange()
{
    //pressed on off button
    if (CheckButtonState(m_pins.m_onOffPin, m_currentState.m_onOffValue))
    {
        //this indicates we pressed the on off button
        m_onOff = !m_onOff;
        if (m_onOff)
        {
            m_currentEffect = &m_effects[0]; //We dont want off to be our effect here
            
            m_lcd.display();//Turn LCD display on again if it was off.
        }
        else
        {
            m_currentEffect = &m_effects[Effects::Off];
        }
        return true;
    }

    //This is only allowed if the strip is set to be on
    if (m_onOff)
    {
        //pressed selection button so up the 
        if (CheckButtonState(m_pins.m_selectionPin, m_currentState.m_selectionValue))
        {
            m_currentEffect = &m_effects[static_cast<Effects>((m_currentEffect->m_index + 1) % static_cast<int>(Effects::Off))];
            return true;
        }
    }

    return m_shouldContinouslyUpdateEffect;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool EffectController::UpdateBrightness()
{
    uint16_t potentioValue = analogRead(m_pins.m_brightnessControllPin);
    //since this can occilate we need to ignore a single value change
    
    int mappedValue = map(potentioValue, 0, 1023, 0, 255);
    if (m_brightnessValue != mappedValue && (potentioValue > m_potentioValue + 1 || potentioValue < m_potentioValue - 1))
    {
        m_brightnessValue = static_cast<uint8_t>(mappedValue);
        m_potentioValue = potentioValue;
        m_ledStrip.setBrightness(m_brightnessValue);

        m_ledStrip.show();

        updateBrightnessPercentage();

        return true;
    }

    return false;
}

///-----------------------------------------------------------------------------
///! @brief   Helper to see if we pressed a button since the last loop
///! @remark  This function doesnt care about the change from a release in state
///-----------------------------------------------------------------------------
bool EffectController::CheckButtonState(uint8_t pin, bool& oldValue) const
{
    int currentValue = digitalRead(pin);
    if (currentValue > 0) //button pressed
    {
        bool tempOld = oldValue;
        oldValue = currentValue;
        return static_cast<bool>(currentValue) != tempOld; //has the value changed
    }
    else
    {
        oldValue = false;
    }

    return false;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void EffectController::ShowSolidColor(uint32_t color)
{
    for (uint16_t counter = 0; counter < m_ledStrip.numPixels(); ++counter)
    {
        m_ledStrip.setPixelColor(counter, color);
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void EffectController::DisplayEffectName()
{
    //These should match the names in Effects
    m_lcd.clear();
    m_lcd.setCursor(0, 0);
    m_lcd.print("DESK LED LIGHT");
    m_lcd.setCursor(0, 1); //Second line on the display
    
    m_lcd.print(m_currentEffect->m_name);

    updateBrightnessPercentage();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void EffectController::updateBrightnessPercentage()
{
    uint8_t endPosEffectName = strlen(m_currentEffect->m_name);
    if (endPosEffectName < 12)
    {
        //we have 4 chars left on the line, put the brightness percentage there
        uint8_t percentage = map(m_brightnessValue, 0, 255, 0, 100);
        m_lcd.setCursor(12, 1);
        if (10 < percentage && percentage < 100)
        {
            m_lcd.print(" ");
        }
        else if (percentage < 10)
        {
            m_lcd.print("  ");
        }
        
        m_lcd.print(percentage);
        m_lcd.print("%");
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark THe color to pass in has to be in the format the strip expects
///-----------------------------------------------------------------------------
void EffectController::KnightRider(uint32_t windowSize, uint32_t color)
{
    static int moveDirection = 1; //start of moving to the right
    static float position = 0.3f;
    static float moveAmount = 0.75f;

    m_ledStrip.clear();

    //Either we are at the start or end of the strip so invert direction
    if (position < 0.2f || position > m_numberOfLeds)
    {
        moveDirection = moveDirection * -1; //Invert direction;
        position = position < 0.2f ? 0.3f : 118.8f; //return to the beginning of the strip
    }
    else
    {
        //Move the LED block by one led
        Color colorPassing;
        colorPassing.m_wrgb = color;
        DrawPixels(position, windowSize, colorPassing, reinterpret_cast<Color*>(m_ledStrip.getPixels()), m_numberOfLeds);
        position = position + (static_cast<int>(windowSize) * moveDirection * moveAmount);
    }

    Serial.println(position);

    delay(50);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void EffectController::Twinkle()
{
    uint32_t colors[] =
    { 
        m_ledStrip.Color(255, 0, 0, 0),
        m_ledStrip.Color(0, 255, 0, 0),
        m_ledStrip.Color(0, 0, 255, 0),
        m_ledStrip.Color(0, 0, 0, 255),
    };



    static int callCount = 0;
    if (callCount >= m_numberOfLeds / 4)
    {
        callCount = 0;
        m_ledStrip.clear();
        Serial.println("Clear Strip");

    }
    else
    {
        int position = random(m_numberOfLeds); //There are 4 elements to the GRBW colors
        //Serial.println(position);
        uint8_t* pixels = m_ledStrip.getPixels();
        uint32_t* fullColorPixels = (uint32_t*)(pixels);
        fullColorPixels[position] = colors[random(sizeof(colors) / sizeof(uint32_t))];
        ++callCount;
    }

    Serial.println(callCount);
    delay(50);

}
