class WINDOW : AccelStepper
{
public:
    int windowCurrentPositiopn = 30 * 200 * 3.5;

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
        pinMode(motorPowerPin, OUTPUT);
        digitalWrite(motorPowerPin, HIGH); // off the motor
        // switch responsible for define when window close
        pinMode(SwitchPin, INPUT);
        //  Serial.print("Switch is  ");
        //  Serial.println(digitalRead(SwitchPin));
        switchPin = SwitchPin;
        setMaxSpeed(1000);
        setAcceleration(800);
    }

    void WindowClose()
    {
        for (int i = 0; i < 20; i++)
        {
            ReadSwitch = ReadSwitch + digitalRead(switchPin);
            //  Serial.print(ReadSwitch);
        }
        ReadSwitch = round(ReadSwitch / 20);

        if ((FirstTimeFlag || windowNewPositiopn == 0 && internetEvent) && !ReadSwitch)
        {
            // run by constant speed
            digitalWrite(motorPowerPin, LOW);
            setMaxSpeed(1000);
            setAcceleration(800);
            setSpeed(1000);
            runSpeed();
        }
        if (ReadSwitch)
        {
            windowCurrentPositiopn = 30 * 200 * 3.5;
            FirstTimeFlag = 0;
        }
        if (ReadSwitch && (FirstTimeFlag || windowNewPositiopn == 0))
        {
            digitalWrite(motorPowerPin, HIGH);
            windowCurrentPositiopn = 0;
            setCurrentPosition(0);
            FirstTimeFlag = 0;
            internetEvent = 0;
            //    Serial.println("ALL OVER");
            //  delay(10000);
        }
    }

    void WindowOpen()
    {
        for (int i = 0; i < 10; i++)
        {
            ReadSwitch = ReadSwitch + digitalRead(switchPin);
        }
        ReadSwitch = round(ReadSwitch / 10);
        /*   Serial.print("windowCurrentPositiopn_");

           Serial.print(windowCurrentPositiopn);
           Serial.print("  windowNewPositiopn_");
           Serial.print(windowNewPositiopn);
           Serial.print("  internetEvent_");
           Serial.print(internetEvent);
           Serial.print("  ReadSwitch_");
           Serial.print(ReadSwitch);
           Serial.print("  FirstTimeFlag_");
           Serial.println(FirstTimeFlag);
          */
        if (windowCurrentPositiopn != windowNewPositiopn && windowNewPositiopn != 0 && internetEvent && !FirstTimeFlag)
        {
            Serial.print("WindowOpen  12");
            setCurrentPosition(windowCurrentPositiopn); // new line
            windowNewPositiopn = map(windowNewPositiopn, 0, 100, 0, round(30 * 200 * 3.5));
            digitalWrite(motorPowerPin, LOW);
            moveTo(windowNewPositiopn);
            runToPosition();
            digitalWrite(motorPowerPin, HIGH);
            setCurrentPosition(windowNewPositiopn);
            windowCurrentPositiopn = windowNewPositiopn;
            Internet_flag = 0;
            Serial.print("WindowOpening  ");
        }
    }
    // process event from server page
    void windowGetValueFromInternet(String value)
    {
        windowNewPositiopn = 100 - value.toInt();
        if (windowNewPositiopn > 95)
            windowNewPositiopn = 0;
        else if (windowNewPositiopn < 5)
            windowNewPositiopn = 1;
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