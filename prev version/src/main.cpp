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
int currentMin;
int Internet_flag = 0;
// bool FirstTimeFlag = 1;
bool FirstTimeFlag = 1;
bool internetEvent = 0;
const int leng = 5;
// WIFI ADJUSTs
bool sheduleFlag = 0;
bool permission = 1;
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
#include "window.h"

//   WINDOW(int motorInterfaceTyp, int step, int dir, int enable, int SwitchPin, String NAME) : AccelStepper(motorInterfaceTyp, step, dir)

WINDOW myWindow = WINDOW(motorInterfaceType, 12, 14, 13, 16, "window");

// REPLACE WITH RECEIVER MAC Address EC:FA:BC:04:1D:20
uint8_t MoonBath[] = {0xEC, 0xFA, 0xBC, 0x04, 0x1D, 0x20};
uint8_t hall[] = {0xEC, 0xFA, 0xBC, 0x04, 0x1D, 0x20};
#include "led.h"
// in
SimpleObject red(5, "redColor", 1);
SimpleObject blue(15, "greenColor", 1);
SimpleObject green(4, "blueColor", 1);
// SimpleObject PlantAtMyRoom(2, "PlantAtMyRoom", 1);
// out
SimpleObject PlantAtMyHall(0, "PlantAtHall", 1);
SimpleObject Moon(99, "Moon", 0);
SimpleObject Bath(99, "Bath", 0);

#include "WifiAdjust.h"
int shedulepin = 2;
void setup()
{
    pinMode(shedulepin, OUTPUT);
    digitalWrite(shedulepin, LOW);
    Serial.begin(115200);
    WiFiSetup();
    Serial.println("WiFi succses");
    Serial.println("WiFi succses");

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
    if (currentMin != timeClient.getMinutes())
    {
        currentMin = timeClient.getMinutes();
        // get temperature;
    }
    myWindow.WindowClose();
    myWindow.WindowOpen();

    // myWindow.scheduleAction(hourInLive, timeClient.getHours());
    if (timeClient.getMinutes() == 55 && sheduleFlag && myWindow.windowCurrentPositiopn == 0 && permission)
    {
        digitalWrite(shedulepin, HIGH);
        Serial.print("Shedule open  ");
        Serial.println(timeClient.getHours());
        sheduleFlag = 0;
        // open window
        // internetEvent = 1;
        // myWindow.windowNewPositiopn = 10 * 200 * 5;
    }
    if (timeClient.getMinutes() == 0 && !sheduleFlag && permission)
    {
        sheduleFlag = 1;
        digitalWrite(shedulepin, LOW);
        Serial.print("Shedule close  ");
        Serial.println(timeClient.getHours());
        // close window
        //       internetEvent = 1;
        //        myWindow.windowNewPositiopn = 0;
    }
}
