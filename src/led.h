
class SimpleObject
{
public:
    // const char topic = "/slider";
    int topicValue;
    char *topic = "/update";
    String name;
    int pin;
    int state;
    bool mode;
    uint8_t broadcastAddress[6];
    // declaring object, his topic and pin
    SimpleObject(int pinout, String UnderTopic, bool position)
    {
        name = UnderTopic;
        pinMode(pinout, OUTPUT);
        digitalWrite(pinout, HIGH);
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
    void GetValueFromInternet(String value)
    {
        state = 255 - value.toInt();
        Serial.print(pin);
        Serial.print("  pin  ");
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