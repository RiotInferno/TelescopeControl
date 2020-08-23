#include <U8x8lib.h>
#include <SoftwareSerial.h>

class Display
{
public:
    Display()
    {
        oled = U8X8_SSD1306_128X64_NONAME_HW_I2C(/* reset=*/U8X8_PIN_NONE);
    }

    void Setup(bool debug = false);
    void print(String data);
    void println(String data);
    U8X8_SSD1306_128X64_NONAME_HW_I2C oled;

private:
    bool debug = false;
};