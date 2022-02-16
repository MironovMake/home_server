

#if defined(ESP8266)
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>
#endif
#include <ESPAsyncTCP.h>
#include <Arduino_JSON.h>
#include <AsyncElegantOTA.h>
#include <Hash.h>
#include <ESPAsyncWebServer.h>

#include <FS.h>
#include <LittleFS.h>
#include <DNSServer.h>

// #include <WiFiManager.h>

#include <ESPAsyncWiFiManager.h> //https://github.com/tzapu/WiFiManager

//=====================//
// Set your Static IP address
IPAddress local_IP(192, 168, 1, 2);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

const char *ssid = "iamlookingatyou";
const char *password = "dUNFcUEP";

AsyncWebServer server(80);
DNSServer dns;

// Create an Event Source on /events
AsyncEventSource events("/events");

// REPLACE WITH YOUR NETWORK CREDENTIALS

String GeneralString;
const char *MyFile = {"/MyFile.txt"};
String TemporaryFile1;
String TemporaryFile2;
bool InitFlag = 0;
const char *parametr[leng] = {
    "value", "BUTTONPLACEHOLDER", "RGB", "slider", "BRIGHTNESS", // 5
};                                                               // corner to turn
String sliderValue = "0";

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char *path)
{
  File file = fs.open(path, "r");
  if (!file || file.isDirectory())
  {
    return String();
  }
  String fileContent;
  while (file.available())
  {
    fileContent += String((char)file.read());
  }
  return fileContent;
}

void writeFile(fs::FS &fs, const char *path, String message)
{
  File file = fs.open(path, "w");
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
  File file = fs.open(path, "w");
}

String processor2(const String &var)
{
  for (int j = 0; j < leng; j++)
  {
    if (var == parametr[j])
    {
      TemporaryFile1 = parametr[j];
      TemporaryFile2 = "";
      if (j != leng - 1)
      {
        for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.indexOf(parametr[j + 1]); i++)
        {
          TemporaryFile2 += GeneralString[i];
        }
      }
      else if (j == leng - 1)
      {
        for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.length(); i++)
        {
          TemporaryFile2 += GeneralString[i];
        }
      }
      return TemporaryFile2;
    }
  }
  return String();
}

String GettingValueFromString(int j)
{
  TemporaryFile1 = parametr[j];
  TemporaryFile2 = "";
  if (j != leng - 1)
  {
    for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.indexOf(parametr[j + 1]); i++)
    {
      TemporaryFile2 += GeneralString[i];
    }
  }
  else if (j == leng - 1)
  {
    for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.length(); i++)
    {
      TemporaryFile2 += GeneralString[i];
    }
  }
  return TemporaryFile2;
}

void SendingValueToString(int j, int k)
{
  TemporaryFile1 = parametr[j];
  TemporaryFile2 = "";
  if (j != leng - 1)
  {
    for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.indexOf(parametr[j + 1]); i++)
    {
      TemporaryFile2 += GeneralString[i];
    }
  }
  else if (j == leng - 1)
  {
    for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.length(); i++)
    {
      TemporaryFile2 += GeneralString[i];
    }
  }
  GeneralString.replace(parametr[j] + TemporaryFile2, parametr[j] + String(k));
}

void WiFiSetup()
{

  Serial.println("ESP8266 INIT");
  Serial.println(F("Inizializing FS..."));
  (LittleFS.begin()) ? Serial.println(F("done.")) : Serial.println(F("fail."));

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("STA Failed to configure");
  }

  // my date
  GeneralString = readFile(LittleFS, MyFile);
  bool IKnow = 1;
  if (IKnow)
  {
    GeneralString = "";
    for (int i = 0; i < leng - 1; i++)
    {
      GeneralString += parametr[i] + String(0);
    }
    GeneralString += parametr[leng - 1] + String(254);
    writeFile(LittleFS, MyFile, GeneralString);
  }
  else
  {
    for (int i = 0; i < leng; i++)
    {
      // InternetSensorState[i] = 0;
      // CurrentSensorState[i] = GettingValueFromString(i).toInt();
    }
  }
  Serial.println("GeneralString ");
  Serial.println(GeneralString);

  // work with wifi
  //

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);

    AsyncWiFiManager wifiManager(&server, &dns);
    wifiManager.autoConnect("AutoConnectAP");
  }
  Serial.println("connected...yeey :)");
  Serial.println(WiFi.localIP());
  AsyncElegantOTA.begin(&server); // Start ElegantOTA

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html"); });

  // get date from slider for window position
  server.serveStatic("/", LittleFS, "/");
  server.on(myWindow.topic, HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String inputMessage;
              if (request->hasParam(myWindow.name)) // changed
              {
                inputMessage = request->getParam(myWindow.name)->value();
                myWindow.windowGetValueFromInternet(inputMessage);
                Serial.print("Switch  ");
                Serial.println(digitalRead(16));
              }
              else if (request->hasParam(PlantAtMyHall.name))
              {
                inputMessage = request->getParam(PlantAtMyHall.name)->value();
                PlantAtMyHall.GetValueFromInternet(inputMessage);
                // int a = PlantAtMyHall.state;
                hallPlantSend.DATE=PlantAtMyHall.state;
                hallPlantSend.PIN=PlantAtMyHall.pin;
                Serial.print(hallPlantSend.DATE);
                Serial.print("  ");
                Serial.print(hallPlantSend.PIN);
                Serial.print("  ");
                for (int i = 0; i <7;i++)
                esp_now_send(hall, (uint8_t *)&hallPlantSend, sizeof(hallPlantSend));
                Serial.println("It's changed");
           //     ESPNow.send_message(receiver_mac, &a, 1);
              }
              else if (request->hasParam(red.name)) // changed
              {
                inputMessage = request->getParam(red.name)->value();
                red.GetValueFromInternet(inputMessage);
                red.ObjectAction(red.state);
              }
              else if (request->hasParam(green.name)) // changed
              {
                inputMessage = request->getParam(green.name)->value();
                green.GetValueFromInternet(inputMessage);
                green.ObjectAction(green.state);
              }
              else if (request->hasParam(blue.name)) // changed
              {
                inputMessage = request->getParam(blue.name)->value();
                blue.GetValueFromInternet(inputMessage);
                blue.ObjectAction(blue.state);
              }else if (request->hasParam(Moon.name)) // changed
              {
                inputMessage = request->getParam(Moon.name)->value();
                Moon.GetValueFromInternet(inputMessage);
                Moon.ObjectAction(Moon.state);
                MoonSend.DATE=Moon.state;
                MoonSend.PIN=Moon.pin;
                esp_now_send(MoonBath, (uint8_t *)&MoonSend, sizeof(MoonSend));
              }
              else if (request->hasParam(Bath.name)) // changed
              {
                inputMessage = request->getParam(Bath.name)->value();
                Bath.GetValueFromInternet(inputMessage);
                Bath.ObjectAction(Bath.state);
                BathSend.DATE=Bath.state;
                BathSend.PIN=Bath.pin;
                esp_now_send(MoonBath, (uint8_t *)&BathSend, sizeof(BathSend));
              }
              else if (request->hasParam("MyWindow")) // changed
              {
                inputMessage = request->getParam("MyWindow")->value();
                permission =!permission ;
                Serial.print("sheduleFlag  ");
                Serial.println(permission );
              }
              Internet_flag = 1;
              request->send(200, "text/text", inputMessage); });
  // Start server
  server.begin();
  server.addHandler(&events);

// other adjust
#ifndef ESP8266
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB
#endif
}