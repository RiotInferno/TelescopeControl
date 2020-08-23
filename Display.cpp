#include "display.h"

void Display::Setup(boolean _debug)
{
    debug = _debug;
    oled.begin();
    oled.setPowerSave(0);
    oled.setFont(u8x8_font_chroma48medium8_r);
}

void Display::println(String data)
{
    oled.println(data);
    if (debug)
    {
        Serial.println(data);
    }
}

void Display::print(String data){
    oled.print(data);
    if (debug)
    {
        Serial.print(data);
    }
}