/*
 Name:		DeskRGBLights.ino
 Created:	14-Mar-22 23:34:23
 Author:	Paul Kruitz
*/

#include "Color.h"
#include "EffectController.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
EffectController controller(lcd, PinConnections(), 119);
//LiquidCrystal lcd;

uint16_t m_elapsedTime = 0;
uint16_t m_oldTimeStamp = 0;

////Effect names, no more than 16 chars each
//char* messages[] =
//{
//    "RAINBOW"
//};

void ResetTimerForLCDBlanking()
{
    lcd.display();
    m_elapsedTime = 0;
    m_oldTimeStamp = millis();

    //Serial.println("void ResetTimerForLCDBlanking()");
}

// the setup function runs once when you press reset or power the board
void setup() 
{
    //Serial.begin(9600);

    controller.Initialise();
}

// the loop function runs over and over again until power down or reset
void loop()
{

    if (controller.UpdateBrightness())
    {
        ResetTimerForLCDBlanking();
    }

    if (controller.UpdateEffectChange())
    {
        controller.SelectAndShowEffect();
        ResetTimerForLCDBlanking();
    }

    //Turn the LCD display off after a while when the strip is not active
    unsigned long currentTimeStamp = millis();
    m_elapsedTime += (currentTimeStamp - m_oldTimeStamp);
    m_oldTimeStamp = currentTimeStamp;
    if (m_elapsedTime > 60000) //about a minute
    {
        //Turn off LCD display
        lcd.noDisplay();
        //Serial.println("No Display");
    }
    else
    {
      //Serial.println("Displaying");
    }
}
