#define M5STACK_MPU6886

#include <M5Stack.h>

// constants SSID, PW, IP and PORT are set during compilation
// they have to be converted to strings with those macros:
#define XSTR(x) #x
#define STR(x) XSTR(x)

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

float temp = 0.0F;

bool buttonA = false;
bool buttonB = false;
bool buttonC = false;

void printConnectionCredentials()
{
    M5.Lcd.setCursor(20, 20);
    M5.Lcd.print("SSID: ");
    M5.Lcd.print(STR(SSID));

    M5.Lcd.setCursor(20, 40);
    M5.Lcd.print("Password: ");
    M5.Lcd.print(STR(PW));

    M5.Lcd.setCursor(20, 60);
    M5.Lcd.print("IP: ");
    M5.Lcd.print(STR(IP));

    M5.Lcd.setCursor(20, 80);
    M5.Lcd.print("Port: ");
    M5.Lcd.print(STR(PORT));
}

void printSensorValues()
{
    M5.Lcd.setCursor(20, 100);
    M5.Lcd.printf("Accelerometer: X %.2f | Y %.2f | Z %.2f", accX, accY, accZ);

    M5.Lcd.setCursor(20, 120);
    M5.Lcd.printf("Gyroscope:     X %.2f | Y %.2f | Z %.2f", gyroX, gyroY, gyroZ);

    M5.Lcd.setCursor(20, 140);
    M5.Lcd.printf("Angle:         X %.2f | Y %.2f | Z %.2f", pitch, roll, yaw);

    M5.Lcd.setCursor(20, 160);
    M5.Lcd.printf("Temperature:   %.2f Celsius", temp);
}

void printButtonStates()
{
    M5.Lcd.setCursor(20, 180);
    M5.Lcd.printf("Buttons: A: %4s B: %4s C: %4s",
                  buttonA ? "down" : "up",
                  buttonB ? "down" : "up",
                  buttonC ? "down" : "up");
}

void setup()
{
    M5.begin();
    M5.Power.begin();
    M5.IMU.Init();
    M5.Lcd.setTextSize(1.5);
    printConnectionCredentials();
}

void loop()
{
    M5.update();

    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    M5.IMU.getAhrsData(&pitch, &roll, &yaw);
    M5.IMU.getTempData(&temp);

    buttonA = M5.BtnA.isPressed();
    buttonB = M5.BtnB.isPressed();
    buttonC = M5.BtnC.isPressed();

    printSensorValues();
    printButtonStates();

    delay(10);
}
