#define M5STACK_MPU6886

#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// constants SSID, PW, IP and PORT are set during compilation
// they have to be converted to strings with those macros:
#define XSTR(x) #x
#define STR(x) XSTR(x)

#define DISCONNECTED 0
#define CONNECTING 1
#define CONNECTED 2

bool SEND_OVER_SERIAL = 1;
bool SEND_OVER_UDP = 1;

WiFiUDP udp;

char* SSID;
char* PW;
char* IP;
uint16_t PORT;

const char* ConnectionStates[3] = {
    "DISCONNECTED",
    "CONNECTING",
    "CONNECTED"
};

uint8_t connectionState = DISCONNECTED;

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

float temperature = 0.0F;

const int NUM_BUTTONS = 3;
bool buttons[NUM_BUTTONS];

void showErrorMessage(String message)
{
    M5.Lcd.fillRect(0, 0, 320, 240,0);
    M5.Lcd.setCursor(20, 20);
    M5.Lcd.print(message);
}

void printConnectionCredentials()
{
    M5.Lcd.setCursor(20, 20);
    M5.Lcd.print("SSID: ");
    M5.Lcd.print(SSID);

    M5.Lcd.setCursor(20, 40);
    M5.Lcd.print("Password: ");
    M5.Lcd.print("*******");
    //M5.Lcd.print(PW);

    M5.Lcd.setCursor(20, 60);
    M5.Lcd.print("IP: ");
    M5.Lcd.print(IP);

    M5.Lcd.setCursor(20, 80);
    M5.Lcd.print("Port: ");
    M5.Lcd.print(PORT);
}

void printConnectionState()
{
    M5.Lcd.setCursor(20, 100);
    M5.Lcd.printf("%12s", ConnectionStates[connectionState]);
}

void printSensorValues()
{
    M5.Lcd.setCursor(20, 120);
    M5.Lcd.printf("Accelerometer: X %.2f | Y %.2f | Z %.2f", accX, accY, accZ);

    M5.Lcd.setCursor(20, 140);
    M5.Lcd.printf("Gyroscope:     X %.2f | Y %.2f | Z %.2f", gyroX, gyroY, gyroZ);

    M5.Lcd.setCursor(20, 160);
    M5.Lcd.printf("Angle:         X %.2f | Y %.2f | Z %.2f", pitch, roll, yaw);

    M5.Lcd.setCursor(20, 180);
    M5.Lcd.printf("Temperature:   %.2f Celsius", temperature);
}

void printButtonStates()
{
    M5.Lcd.setCursor(20, 200);
    M5.Lcd.printf("Buttons: A: %4s B: %4s C: %4s",
                  buttons[0] ? "down" : "up",
                  buttons[1] ? "down" : "up",
                  buttons[2] ? "down" : "up");
}

void connectToWiFi()
{
    printConnectionState();

    // sending over WiFi is disabled, e.g. if WiFi credentials are not set
    // don't even try to connect and send data only via serial
    if(!SEND_OVER_UDP) return;

    WiFi.mode(WIFI_STA);

    delay(100);

    WiFi.begin(SSID, PW);

    connectionState = CONNECTING;

    while (true)
    {
        printConnectionState();
        if(WiFi.status() == WL_CONNECTED) break;
        delay(100);
    }

    udp.begin(PORT);
    connectionState = CONNECTED;
    printConnectionState();
}

void readSensorData()
{
    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    M5.IMU.getAhrsData(&pitch, &roll, &yaw);
    M5.IMU.getTempData(&temperature);

    buttons[0] = M5.BtnA.isPressed();
    buttons[1] = M5.BtnB.isPressed();
    buttons[2] = M5.BtnC.isPressed();
}

void sendTemperatureData()
{
    String buffer = String("{\"temperature\":") + temperature + "}";

    if(SEND_OVER_UDP)
    {
        udp.beginPacket(IP, PORT);
        udp.write((uint8_t*)buffer.c_str(), buffer.length());
        udp.endPacket();
    }

    if(SEND_OVER_SERIAL)
    {
        Serial.println(buffer);
    }
}

void sendRotationData()
{
    String buffer = String("{\"rotation\":{") +
                            "\"pitch\":" + pitch + "," +
                            "\"roll\":"  + roll  + "," +
                            "\"yaw\":"   + yaw   + "}}";

    if(SEND_OVER_UDP)
    {
        udp.beginPacket(IP, PORT);
        udp.write((uint8_t*)buffer.c_str(), buffer.length());
        udp.endPacket();
    }

    if(SEND_OVER_SERIAL)
    {
        Serial.println(buffer);
    }
}

void sendGyroscopeData()
{
    String buffer = String("{\"gyroscope\":{") +
                            "\"x\":" + gyroX + "," +
                            "\"y\":" + gyroY + "," +
                            "\"z\":" + gyroZ + "}}";

    if(SEND_OVER_UDP)
    {
        udp.beginPacket(IP, PORT);
        udp.write((uint8_t*)buffer.c_str(), buffer.length());
        udp.endPacket();
    }

    if(SEND_OVER_SERIAL)
    {
        Serial.println(buffer);
    }
}

void sendAccelerometerData()
{
    String buffer = String("{\"accelerometer\":{") +
                            "\"x\":" + accX + "," +
                            "\"y\":" + accY + "," +
                            "\"z\":" + accZ + "}}";

    if(SEND_OVER_UDP)
    {
        udp.beginPacket(IP, PORT);
        udp.write((uint8_t*)buffer.c_str(), buffer.length());
        udp.endPacket();
    }

    if(SEND_OVER_SERIAL)
    {
        Serial.println(buffer);
    }
}

void sendButtonStates()
{
    for(int i = 0; i < NUM_BUTTONS; i++)
    {
        String buffer = String("{\"button_") + (i + 1) + "\":" + buttons[i] + "}"; 

        if(SEND_OVER_UDP)
        {
            udp.beginPacket(IP, PORT);
            udp.write((uint8_t*)buffer.c_str(), buffer.length());
            udp.endPacket();
        }

        if(SEND_OVER_SERIAL)
        {
            Serial.println(buffer);
        }
    }
}

bool readCredentialsFromSD()
{
    // initialize SD card reader
    if (!SD.begin(4)) {
        // show error if the SD card could not be read
        // for example when it's missing
        showErrorMessage("could not read SD card");
        return false;
    }

    // open 'credentials' file
    // the file has to be called 'credentials' (without file extension)
    // it has to be placed in the root directory of the SD card
    // first line is the WiFi SSID
    // second line is the WiFi password
    // third line is the host's IP address
    // fourth line is the host's port
    File credentialsFile = SD.open("/credentials");

    // show error when the file is missing
    if(!credentialsFile)
    {
        showErrorMessage("couldn't open file 'credentials' on SD card");
        return false;
    }

    String buffer[4]; // buffer for the four lines of the file
    buffer[0] = "";   // initialize the first buffer
    int line = 0;     // index of the buffer

    // read the credentials file character by character
    // and store each line in a separate buffer
    while(credentialsFile.available())
    {
        char c = (char) credentialsFile.read(); // read one character from the file

        if(c == '\n') // reached end of the line
        {
            line++; // use the next buffer for the next line
            if(line > 3) break; // stop after four lines
            buffer[line] = ""; // initialize the next buffer
            continue; // we don't want to append the line break to the buffer
        }

        buffer[line].concat(c); // append the read character to the current buffer
    }

    // allocate memory for the WiFi credentials
    SSID = (char*) malloc(buffer[0].length() + 1);
    PW = (char*) malloc(buffer[1].length() + 1);
    IP = (char*) malloc(buffer[2].length() + 1);

    // copy the buffered strings to the global variables for WiFi credentials
    buffer[0].toCharArray(SSID, buffer[0].length() + 1);
    buffer[1].toCharArray(PW, buffer[1].length() + 1);
    buffer[2].toCharArray(IP, buffer[2].length() + 1);
    PORT = buffer[3].toInt();

    return true;
}

void defineWifiCredentials()
{
#ifdef _SSID
    // if credentials have been set during pre-compilation, use those
    SSID = STR(_SSID);
    PW = STR(_PW);
    IP = STR(_IP);
    PORT = _PORT;
#else
    // else, try to read credentials from the SD card
    if(!readCredentialsFromSD())
    {
        // credentials could not be read from SD card
        // deactivate sending data over WiFi
        SEND_OVER_UDP = false;
    }
#endif
}

void setup()
{
    M5.begin();
    Wire.begin();
    M5.Power.begin();
    M5.IMU.Init();
    M5.Lcd.setTextSize(1.5);
    defineWifiCredentials();
    printConnectionCredentials();
    connectToWiFi();
}

void loop()
{
    M5.update();
    sendButtonStates();
    printButtonStates();

    readSensorData();

    sendAccelerometerData();
    sendGyroscopeData();
    sendRotationData();
    sendTemperatureData();

    printSensorValues();

    delay(1);
}
