/*
 Name:		DeskRGBLights.ino
 Created:	14-Mar-22 23:34:23
 Author:	Paul Kruitz
*/

#include "EffectController.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
EffectController controller(lcd, PinConnections(), 119);
//LiquidCrystal lcd;

////Effect names, no more than 16 chars each
//char* messages[] =
//{
//    "RAINBOW"
//};


// the setup function runs once when you press reset or power the board
void setup() 
{
    controller.Initialise();
}

// the loop function runs over and over again until power down or reset
void loop() 
{
    controller.UpdateBrightness();

    if (controller.UpdateEffectChange())
    {
        controller.SelectAndShowEffect();
    }

}
