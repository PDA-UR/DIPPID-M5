#include <M5Stack.h>

// constants SSID, PW, IP and PORT are set during compilation
// they have to be converted to strings with those macros:
#define XSTR(x) #x
#define STR(x) XSTR(x)


void printConnectionCredentials()
{
    M5.Lcd.setTextSize(2);

    M5.Lcd.setCursor(20, 20);
    M5.Lcd.print("SSID: ");
    M5.Lcd.print(STR(SSID));

    M5.Lcd.setCursor(20, 50);
    M5.Lcd.print("Password: ");
    M5.Lcd.print(STR(PW));

    M5.Lcd.setCursor(20, 80);
    M5.Lcd.print("IP: ");
    M5.Lcd.print(STR(IP));

    M5.Lcd.setCursor(20, 110);
    M5.Lcd.print("Port: ");
    M5.Lcd.print(STR(PORT));
}

void setup()
{
    M5.begin();
    M5.Power.begin();
    printConnectionCredentials();
}

void loop()
{

}
