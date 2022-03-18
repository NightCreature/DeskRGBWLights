#pragma once

#include <pins_arduino.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include "ButtonState.h"
#include "Effect.h"

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

enum Effects : uint8_t 
{

    SolidRed = 0,
    SolidGreen,
    SolidBlue,
    SolidWhite,
    Rainbow,

    Off, //This is so we can modulo on this and never get the off state in the loop of effects
    Count
};



//Houses the effects we can run on the strip
class EffectController
{
public:
    //Since the LiquidCrystal code is an asshole
    //EffectController() //:
    //    //m_lcd(m_pins.m_rsPin, m_pins.m_enablePin, m_pins.m_data0Pin, m_pins.m_data1Pin, m_pins.m_data2Pin, m_pins.m_data3Pin) 
    //{}
    //EffectController(const PinConnections& pinsToUse, uint32_t numberOfLeds) :
    //    m_pins(pinsToUse),
    //    m_lcd(m_pins.m_rsPin, m_pins.m_enablePin, m_pins.m_data0Pin, m_pins.m_data1Pin, m_pins.m_data2Pin, m_pins.m_data3Pin)
    //{
    //    m_ledStrip = Adafruit_NeoPixel(numberOfLeds, m_pins.m_ledPin, NEO_GRBW + NEO_KHZ800);
    //}

    EffectController(LiquidCrystal& lcd, const PinConnections& pinToUse, uint32_t numberOfLeds) :
        m_lcd(lcd),
        m_pins(pinToUse),
        m_numberOfLeds(numberOfLeds)
    {
        m_currentEffect = &m_effects[Effects::Off];
        m_brightnessValue = 32; // 1/8th brightness so dim
        m_onOff = true;
    }

    void Initialise();

    void SelectAndShowEffect();

    bool UpdateEffectChange();
    void UpdateBrightness();

    bool isOn() const { return m_onOff; }
private:
    bool CheckButtonState(uint8_t pin, bool& oldValue) const;
    void ShowSolidColor(uint32_t color);
    void DisplayEffectName();
    void updateBrightnessPercentage();

    Adafruit_NeoPixel m_ledStrip;
    LiquidCrystal& m_lcd;
    PinConnections m_pins;
    ButtonState m_currentState;
    Effect m_effects[Effects::Count] =
    {
        {Effects::SolidRed, "Red"},
        {Effects::SolidGreen, "Green"},
        {Effects::SolidBlue, "Blue"},
        {Effects::SolidWhite, "White"},
        {Effects::Rainbow, "Rainbow"},

        //New entries before this
        {Effects::Off, "Off"},
    };
    Effect* m_currentEffect = nullptr;
    uint32_t m_numberOfLeds = 0;
    uint8_t m_brightnessValue; //need to remap a value from 0..1024 range to 0..255
    bool m_onOff = true;
};
