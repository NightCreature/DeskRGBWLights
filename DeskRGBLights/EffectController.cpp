#include "EffectController.h"

#include <Arduino.h>
#include "Color.h"
#include "LedUtilityFunctions.h"

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
EffectController::EffectController(LiquidCrystal& lcd, const PinConnections& pinToUse) :
    m_lcd(lcd),
    m_pins(pinToUse),
    m_effectIndex(-1)
{
    m_currentEffect = nullptr; //Equivalent to off
    m_brightnessValue = 32; // 1/8th brightness so dim

    //Add Effects
    m_effects[0] = new SolidColor("Red", m_ledStrip, Adafruit_NeoPixel::Color(255, 0, 0));
    m_effects[1] = new SolidColor("Green", m_ledStrip, Adafruit_NeoPixel::Color(0, 255, 0));
    m_effects[2] = new SolidColor("Blue", m_ledStrip, Adafruit_NeoPixel::Color(0, 0, 255));
    m_effects[3] = new SolidColor("White", m_ledStrip, Adafruit_NeoPixel::Color(0, 0, 0, 255));
    m_effects[4] = new SolidColor("Super White", m_ledStrip, Adafruit_NeoPixel::Color(255, 255, 255, 255));
    m_effects[5] = new KnightRider(m_ledStrip, 3, Adafruit_NeoPixel::Color(255, 0, 0));
    m_effects[6] = new TwinkleEffect(m_ledStrip);
    m_effects[7] = new XMasTwinkle(m_ledStrip);
    m_effects[8] = new FireworkEffect(m_ledStrip, 3);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void EffectController::Initialise(uint16_t numberOfLeds)
{
    m_ledStrip = Adafruit_NeoPixel(numberOfLeds, m_pins.m_ledPin, NEO_GRBW + NEO_KHZ800);

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
bool EffectController::Update(float elapsedTime)
{
    if (UpdateBrightness())
    {
        updateBrightnessPercentage();
    }

    bool retVal = UpdateEffectChange();
    if (retVal)
    {
        //Activate Effect here, this will also allow us to time out the display for everything
        SelectAndShowEffect();
        DisplayEffectName();
    }
    
    if (m_currentEffect != nullptr)
    {
        m_currentEffect->Update(elapsedTime);
    }

    m_ledStrip.show();

    return retVal;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void EffectController::SelectAndShowEffect()
{
    if (m_currentEffect != nullptr)
    {
        //There is some pointer math here to select the next effect in the array might just keep an index around
        m_currentEffect->Initialise();
    }
    else //off state
    {
        m_ledStrip.clear();
    }
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
        if (m_currentEffect != nullptr)
        {
            m_currentEffect = nullptr; //We dont want off to be our effect here
            m_effectIndex = -1;
        }
        else
        {
            m_effectIndex = 0;
            m_currentEffect = m_effects[m_effectIndex];
            
        }
        return true;
    }

    //This is only allowed if the strip is set to be on
    if (m_currentEffect != nullptr)
    {
        //pressed selection button so up the 
        if (CheckButtonState(m_pins.m_selectionPin, m_currentState.m_selectionValue))
        {
            m_effectIndex = (m_effectIndex + 1) % m_numberOfEffects;
            m_currentEffect = m_effects[m_effectIndex];
            return true;
        }
    }

    return false;
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
void EffectController::DisplayEffectName()
{
    //These should match the names in Effects
    m_lcd.clear();
    m_lcd.setCursor(0, 0);
    m_lcd.print("DESK LED LIGHT");
    m_lcd.setCursor(0, 1); //Second line on the display
    
    if (m_currentEffect != nullptr)
    {
        m_lcd.print(m_currentEffect->GetEffectName());
    }
    else
    {
        m_lcd.print("Off");
    }

    updateBrightnessPercentage();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void EffectController::updateBrightnessPercentage()
{
    uint8_t endPosEffectName = 3;
    if (m_currentEffect != nullptr)
    {
        uint8_t endPosEffectName = strlen(m_currentEffect->GetEffectName());
    }
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