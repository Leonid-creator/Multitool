#ifndef MultitoolOLED_h
#define MultitoolOLED_h

#include <Arduino.h>
#include <OLED_I2C.h>

class MultitoolOLED : OLED
{
    public:
    uint8_t* smallFont;
    uint8_t* bigNumbers;

    MultitoolOLED(uint8_t SDA, uint8_t SCL) : OLED (SDA, SCL)
    {
        begin(SSD1306_128X64);
        extern uint8_t SmallFont[];
        extern uint8_t BigNumbers[];
        smallFont = SmallFont;
        bigNumbers = BigNumbers;
        setFont(smallFont);
    }
    
    /// @brief 
    /// @param cursorPosition 
    void displayMenu(int cursorPosition)
    {
        clrScr();
        setFont(smallFont);
        print("MAIN MENU", CENTER, 0);
        print("Brightness", 10, 10);
        print("Get voltage", 10, 20);
        print("Set voltage (soon)", 10, 30);
        print("Set PWM", 10, 40);
        print(">", 0, cursorPosition*10);
        update();
    }

    /// @brief 
    /// @param pwmValue 
    void displaySetPWM(int pwmValue)
    {
        clrScr();
        print("PWM", CENTER, 0);
        setFont(bigNumbers);
        printNumI(map(pwmValue, 0, 255, 0, 100), CENTER, 20);
        setFont(smallFont);
        update();
    }

    /// @brief 
    /// @param pwmValue 
    void displayPWMIsNotReady(int pwmValue)
    {
        clrScr();
        print("PWM is not ready", CENTER, 0);
        print("set the potentiometer", CENTER, 10);
        print(" to zero position", CENTER, 20);
        printNumI(map(pwmValue, 0, 255, 0, 100), CENTER, 30);
        update();
    }

    /// @brief 
    /// @param brightness 
    void displayGetBrightness(int brightness)
    {
        clrScr();
        print("Brightness", CENTER, 0);
        setFont(bigNumbers);
        printNumI(brightness, CENTER, 20);
        setFont(smallFont);
        update();
    }

    /// @brief 
    /// @param pwmValue 
    /// @param voltage 
    void displayGetVoltage(int pwmValue, int voltage)
    {
        clrScr();
        print("PWM", CENTER, 0);
        printNumI(map(pwmValue, 0, 255, 0, 100), CENTER, 20);
        setFont(bigNumbers);
        printNumI(voltage, CENTER, 30);
        setFont(smallFont);
        update();
    }
};

#endif