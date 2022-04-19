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

#include "Timer.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
EffectController controller(lcd, PinConnections());
Timer timer;

float elapsedTime = 0;


void ResetTimerForLCDBlanking()
{
    lcd.display();
    elapsedTime = 0;
}

// the setup function runs once when you press reset or power the board
void setup() 
{
    Serial.begin(9600);

    controller.Initialise(119);
}

// the loop function runs over and over again until power down or reset
void loop()
{
    timer.Update();
    elapsedTime += timer.GetElapsedTime();
    Serial.println(timer.GetElapsedTime());

    if (controller.Update(timer.GetElapsedTime()))
    {
        ResetTimerForLCDBlanking();
    }

    //Turn the LCD display off after a while when the strip is not active
    if (elapsedTime > 60.0f) //about a minute
    {
        //Turn off LCD display
        lcd.noDisplay();
    }
}
