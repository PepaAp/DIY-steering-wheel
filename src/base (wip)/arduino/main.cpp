#include <Joystick.h>

#define button1 9
#define button2 8
#define button3 7
#define encoderA 2
#define encoderB 3
#define RPWM 6
#define LPWM 5

#define defClutch 650
#define defBreak  970
#define defGas    1300
#define maxClutch 1320
#define maxBreak  1620
#define maxGas    2120

Joystick_ Joystick(0x1, JOYSTICK_TYPE_JOYSTICK, 3, 0, true, false, true, false, false, false, false, true, false, true, true);

volatile int steerValue = 0;
volatile int lastA;

int lastButton1State = HIGH;
int lastButton2State = HIGH;
int lastButton3State = HIGH;

int steerValueJoystick = 0;

uint16_t gasRaw = defGas, brakeRaw = defBreak, clutchRaw = defClutch;

char serialBuffer[64];
uint8_t serialIndex = 0;

void steering() {
    int A = digitalRead(encoderA);
    int B = digitalRead(encoderB);
    if (A != lastA) steerValue += (B != A) ? 1 : -1;
    lastA = A;
}

void parseData(char* dataLine) {
    int g=0,b=0,c=0;
    if (sscanf(dataLine, "%d,%d,%d", &g, &b, &c) != 3) return;
    gasRaw = g;
    brakeRaw = b;
    clutchRaw = c;
}

void setup() {
    lastA = digitalRead(encoderA);
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(button3, INPUT_PULLUP);
    pinMode(encoderA, INPUT);
    pinMode(encoderB, INPUT);
    pinMode(RPWM, OUTPUT);
    pinMode(LPWM, OUTPUT);

    TCCR4B = (TCCR4B & 0b11111000) | 0x01;
    TCCR3B = (TCCR3B & 0b11111000) | 0x01;

    attachInterrupt(digitalPinToInterrupt(encoderA), steering, CHANGE);

    analogWrite(RPWM, 0);
    analogWrite(LPWM, 0);

    Serial.begin(9600);
    Serial1.begin(9600);
    Joystick.begin();
}

void loop() {
    int b1 = digitalRead(button1);
    if (b1 != lastButton1State) { Joystick.setButton(0, b1==LOW); lastButton1State=b1; }
    int b2 = digitalRead(button2);
    if (b2 != lastButton2State) { Joystick.setButton(1, b2==LOW); lastButton2State=b2; }
    int b3 = digitalRead(button3);
    if (b3 != lastButton3State) { Joystick.setButton(2, b3==LOW); lastButton3State=b3; }

    while (Serial1.available()) {
        char c = Serial1.read();
        if (c=='\n') {
            serialBuffer[serialIndex] = 0;
            parseData(serialBuffer);
            serialIndex = 0;
        } else if (serialIndex<sizeof(serialBuffer)-1) serialBuffer[serialIndex++] = c;
    }

    int gasMapped = constrain(map(gasRaw, defGas, maxGas, 0, 1023), 0, 1023);
    int brakeMapped = constrain(map(brakeRaw, defBreak, maxBreak, 0, 1023), 0, 1023);
    int clutchMapped = constrain(map(clutchRaw, defClutch, maxClutch, 0, 1023), 0, 1023);

    Joystick.setThrottle(gasMapped);
    Joystick.setBrake(brakeMapped);
    Joystick.setZAxis(clutchMapped);

    steerValueJoystick = constrain(map(steerValue, -2560, 2560, 0, 1024), 0, 1024);
    int motorRight = map(steerValueJoystick, 513, 1024, 60, 255);
    int motorLeft  = map(steerValueJoystick, 0, 511, 255, 60); // normalne je misto 115 255 ale rozbil se mi motorek


    Serial.println(motorRight);
    if (steerValueJoystick < 512) { analogWrite(LPWM, motorLeft > 115 ? 115: motorLeft); digitalWrite(RPWM, LOW); }
    else if (steerValueJoystick > 512) { analogWrite(RPWM, motorRight > 115 ? 115: motorRight); digitalWrite(LPWM, LOW); }
    else { analogWrite(RPWM, 0); analogWrite(LPWM, 0); }

    Joystick.setXAxis(steerValueJoystick);
    Joystick.sendState();
}
