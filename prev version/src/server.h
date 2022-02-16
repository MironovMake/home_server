#include <Arduino.h>

#include <espnow.h>
typedef struct struct_message
{
    int DATE;
    int PIN;

} struct_message;
struct_message MoonSend;
struct_message BathSend;
struct_message hallPlantSend;

int Internet_flag = 0;
bool FirstTimeFlag = 1;
bool internetEvent;
const int leng = 5;
int flaglisenmotor = 0;
// WIFI ADJUSTs

#include <AccelStepper.h>
#define motorInterfaceType 1

int hourInLive;
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Variable to save current epoch time
unsigned long epochTime;

// Function that gets current epoch time
unsigned long getTime()
{
    timeClient.update();
    unsigned long now = timeClient.getEpochTime();
    return now;
}

class WINDOW : AccelStepper
{
public:
    int windowCurrentPositiopn;
    int windowNewPositiopn;
    char *topic = "/slider";
    int switchPin;
    int ReadSwitch;
    String name;
    WINDOW(int motorInterfaceTyp, int step, int dir, int enable, int SwitchPin, String NAME) : AccelStepper(motorInterfaceTyp, step, dir)
    {
        name = NAME;
        // setup pin which responsible for power supply
        motorPowerPin = enable;
        Serial.println(motorPowerPin);
        pinMode(motorPowerPin, OUTPUT);
        digitalWrite(motorPowerPin, HIGH); // off the motor
        // switch responsible for define when window close
        pinMode(SwitchPin, INPUT);
        Serial.print("Switch is  ");
        Serial.println(digitalRead(SwitchPin));
        switchPin = SwitchPin;
    }

    void WindowClose()
    {
        for (int i = 0; i < 20; i++)
        {
            ReadSwitch = ReadSwitch + digitalRead(switchPin);
            //  Serial.print(ReadSwitch);
        }
        ReadSwitch = round(ReadSwitch / 20);
        if (!flaglisenmotor)
        {
            Serial.print("  ReadSwitch  ");
            Serial.println(ReadSwitch);
            flaglisenmotor++;
        }
        if ((FirstTimeFlag || windowNewPositiopn == 0 && internetEvent) && !ReadSwitch)
        {
            // run by constant speed
            digitalWrite(motorPowerPin, LOW);
            this->setMaxSpeed(600);
            this->setAcceleration(100);
            this->setSpeed(100);
            this->runSpeed();
        }
        else if (ReadSwitch)
        {
            digitalWrite(motorPowerPin, HIGH);
            this->setCurrentPosition(0);
            FirstTimeFlag = 0;
            internetEvent = 0;
            Serial.println("WindowClose else if  ");
        }
    }

    void WindowOpen()
    {
        for (int i = 0; i < 10; i++)
        {
            ReadSwitch = ReadSwitch + digitalRead(switchPin);
        }
        ReadSwitch = round(ReadSwitch / 10);
        if (this->windowCurrentPositiopn != this->windowNewPositiopn && this->windowNewPositiopn != 0 && internetEvent && !ReadSwitch)
        {
            Serial.print("WindowOpen  12");
            this->setCurrentPosition(this->windowCurrentPositiopn); // new line
            this->windowNewPositiopn = map(this->windowNewPositiopn, 0, 100, 0, 30 * 200);
            digitalWrite(motorPowerPin, LOW);
            this->moveTo(windowNewPositiopn);
            this->runToPosition();
            digitalWrite(motorPowerPin, HIGH);
            this->setCurrentPosition(windowNewPositiopn);
            this->windowCurrentPositiopn = this->windowNewPositiopn;
            Internet_flag = 0;
            Serial.print("WindowOpen  ");
        }
        if (ReadSwitch)
        {
            digitalWrite(motorPowerPin, HIGH);
            this->setCurrentPosition(0);
            FirstTimeFlag = 0;
            internetEvent = 0;
            Serial.println("WindowClose ");
        }
    }
    // process event from server page
    void windowGetValueFromInternet(String value)
    {
        windowNewPositiopn = value.toInt();
        if (windowNewPositiopn < 5)
            windowNewPositiopn = 0;
        Serial.print("windowNewPositiopn  ");
        Serial.println(windowNewPositiopn);
    }
    void scheduleAction(int hourNOW, int minNOW)
    {
        for (int i = 0; i < 6; i++)
        {
            if ((minOn != minOff || hourOn[i] != hourOff[i]) && !sheduleflag && minNOW == minOn && hourNOW == hourOn[i])
            {
                windowNewPositiopn = 50;
                sheduleflag = 1;
            }
            if ((minOn != minOff || hourOn[i] != hourOff[i]) && sheduleflag && minNOW == minOff && hourNOW == hourOff[i])
            {
                windowNewPositiopn = 0;
                sheduleflag = 0;
            }
        }
    }

private:
    bool sheduleflag;
    int motorPowerPin;

    int minOn = 0;
    int hourOn[5] = {1, 2, 3, 4, 5};
    int minOff = 5;
    int hourOff[5] = {1, 2, 3, 4, 5};
};
//   WINDOW(int motorInterfaceTyp, int step, int dir, int enable, int SwitchPin, String NAME) : AccelStepper(motorInterfaceTyp, step, dir)

WINDOW myWindow = WINDOW(motorInterfaceType, 12, 14, 13, 16, "window");

// REPLACE WITH RECEIVER MAC Address EC:FA:BC:04:1D:20
uint8_t MoonBath[] = {0xEC, 0xFA, 0xBC, 0x04, 0x1D, 0x20};
uint8_t hall[] = {0xEC, 0xFA, 0xBC, 0x04, 0x1D, 0x20};

class SimpleObject
{
public:
    // const char topic = "/slider";
    int topicValue;
    char *topic = "/update";
    String name;
    int pin;
    bool state;
    bool mode;
    uint8_t broadcastAddress[6];
    // declaring object, his topic and pin
    SimpleObject(int pinout, String UnderTopic, bool position)
    {
        name = UnderTopic;
        pinMode(pinout, OUTPUT);
        pin = pinout;
        // if mode ==1 then its object on the Master esp8266
        // if mode ==0 then its object on the Slave esp8266
        mode = position;
    }

    void ObjectAction()
    {
        digitalWrite(pin, !digitalRead(pin));
    }
    void ObjectAction(int value)
    {
        analogWrite(pin, value);
    }
    void objectGetValueFromInternet(String value)
    {
        this->ObjectAction();
    }
    void scheduleActivity(int hourNow, int minNow)
    {
        if ((minOn != minOff || hourOn != hourOff) && !sheduleflag && minNow == minOn && hourNow == hourOn)
        {
            digitalWrite(pin, HIGH);
            sheduleflag = 1;
        }
        if ((minOn != minOff || hourOn != hourOff) && sheduleflag && minNow == minOff && hourNow == hourOff)
        {
            digitalWrite(pin, LOW);
            sheduleflag = 0;
        }
    }
    // createShedulee(int minon, int houron, int minoff, int houroff)
    void createShedulee(int houron, int minon, int houroff, int minoff)
    {
        minOn = minon;
        hourOn = houron;
        minOff = minoff;
        minOff = houroff;
    }
    void GetValueFromInternet(String value)
    {

        if (value.toInt() == 1)
        {
            state = !state;
        }
        else
        {

            Serial.println(value.toInt());
            state = value.toInt();
        }
        Serial.print(name);
        Serial.print("  state  ");
        Serial.println(state);
    }

private:
    int minOn = 0;
    int hourOn = 20;
    int minOff = 0;
    int hourOff = 8;
    bool sheduleflag;
};
// in
SimpleObject red(15, "redColor", 1);
SimpleObject blue(4, "greenColor", 1);
SimpleObject green(5, "blueColor", 1);
SimpleObject PlantAtMyRoom(2, "PlantAtMyRoom", 1);
// out
SimpleObject PlantAtMyHall(0, "PlantAtHall", 1);
SimpleObject Moon(99, "Moon", 0);
SimpleObject Bath(99, "Bath", 0);
static uint8_t a = 0;
#include "WifiAdjust.h"

void setup()
{
    PlantAtMyRoom.createShedulee(10, 0, 23, 0);
    PlantAtMyRoom.createShedulee(20, 0, 8, 0);

    Serial.begin(115200);
    WiFiSetup();

    Serial.println("WiFi succses");
    // window power
    Serial.println("Finish success");
    timeClient.begin();
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_add_peer(MoonBath, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
    esp_now_add_peer(hall, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop()
{
    timeClient.update();
    (timeClient.getHours() > 24 - 7) ? hourInLive = timeClient.getHours() + 7 - 24 : hourInLive = timeClient.getHours() + 7;

    myWindow.WindowClose();
    myWindow.WindowOpen();

    // myWindow.scheduleAction(hourInLive, timeClient.getHours());
}
