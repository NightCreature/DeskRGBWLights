#pragma once

#include <pins_arduino.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include "ButtonState.h"
#include "Effect.h"
#include "FireworkEffect.h"
#include "TwinkleEffect.h"
#include "KnightRider.h"

struct PinConnections
{
    PinConnections()
    {
        m_ledPin = 10;
        m_selectionPin = 8;
        m_onOffPin = 9;
        m_brightnessControllPin = A0; //this needs to be an analog pin
        m_lcdContrastPin = A1;

        m_rsPin = 12;
        m_enablePin = 11;
        m_data0Pin = 5;
        m_data1Pin = 4;
        m_data2Pin = 3;
        m_data3Pin = 2;
    }

    char m_ledPin = 10;
    char m_selectionPin = 8;
    char m_onOffPin = 9;
    char m_brightnessControllPin = A0; //this needs to be an analog pin
    char m_lcdContrastPin = A1;

    char m_rsPin = 12;
    char m_enablePin = 11;
    char m_data0Pin = 5;
    char m_data1Pin = 4;
    char m_data2Pin = 3;
    char m_data3Pin = 2;
};

//Houses the effects we can run on the strip
class EffectController
{
public:
    EffectController(LiquidCrystal& lcd, const PinConnections& pinToUse);

    void Initialise(uint16_t numberOfLeds);

    bool Update(float elapsedTime);

    void SelectAndShowEffect();

    bool UpdateEffectChange();
    bool UpdateBrightness();
private:
    bool CheckButtonState(uint8_t pin, bool& oldValue) const;
    void DisplayEffectName();
    void updateBrightnessPercentage();

    const static uint8_t m_numberOfEffects = 9; //Modify this when you add a new effect

    Adafruit_NeoPixel m_ledStrip;
    LiquidCrystal& m_lcd;
    PinConnections m_pins;
    ButtonState m_currentState;

    Effect* m_effects[m_numberOfEffects];
    Effect* m_currentEffect = nullptr;
    uint16_t m_potentioValue;
    uint8_t m_brightnessValue; //need to remap a value from 0..1024 range to 0..255
    int8_t m_effectIndex;
};
