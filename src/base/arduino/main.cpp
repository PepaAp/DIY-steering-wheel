#include <Joystick.h> 

#define button1 9
#define button2 8
#define button3 7
#define clutchPin A1
#define breakPin A2
#define gasPin A3
#define encoderA 2
#define encoderB 3
#define RPWM 6
#define LPWM 5

Joystick_ Joystick(0x1, JOYSTICK_TYPE_JOYSTICK, 3, 0, true, false, true, false, false, false, false, true, false, true, true);

int lastButton1State = true;
int lastButton2State = true;
int lastButton3State = true;
int steerValue = 0;
int steerValueJoystick = 0;
int lastA;

void steering() {
    int A = digitalRead(encoderA);
    int B = digitalRead(encoderB);

    if (A != lastA) {
        if (B != A) {
            steerValue++;
        } else {
            steerValue--;
        }
    }
    lastA = A;
}

void setup() {
    lastA = digitalRead(encoderA);
    Serial.begin(9600);

    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(button3, INPUT_PULLUP);
    pinMode(clutchPin, INPUT);
    pinMode(breakPin, INPUT);
    pinMode(gasPin, INPUT);
    pinMode(encoderA, INPUT);
    pinMode(encoderB, INPUT);
    pinMode(RPWM, OUTPUT);
    pinMode(LPWM, OUTPUT);    
    TCCR4B = (TCCR4B & 0b11111000) | 0x01;
    TCCR3B = (TCCR3B & 0b11111000) | 0x01;
    attachInterrupt(digitalPinToInterrupt(encoderA), steering, CHANGE); 
    analogWrite(RPWM, 0);
    analogWrite(LPWM, 0);
    Joystick.begin();
    delay(5);
}

void loop() {
    int button1State = digitalRead(button1);
    int button2State = digitalRead(button2);
    int button3State = digitalRead(button3);
    int clutchValue = map(analogRead(clutchPin), 280, 400, 0, 1023);
    int breakValue = map(analogRead(breakPin), 440, 550, 0, 1023);
    int gasValue = map(analogRead(gasPin), 320, 450, 0, 1023);
    int motorRight = map(steerValueJoystick, 513, 1024, 60, 255);
    int motorLeft = map(steerValueJoystick, 0, 511, 255, 60);
    /* digitalWrite(LPWM, LOW);
    analogWrite(RPWM, 255); */

    Joystick.setZAxis(clutchValue);
    Joystick.setThrottle(gasValue);
    Joystick.setBrake(breakValue);

    steerValueJoystick = constrain(map(steerValue, -2560, 2560, 0, 1024), 0, 1024);

    
    Serial.print("raw value ");
    Serial.print(steerValue);
    Serial.print(" mapped value ");
    Serial.println(steerValueJoystick);
    
    /* Serial.println(gasValue);
    Serial.println(breakValue);
    Serial.println(clutchValue); */
    
    if (steerValueJoystick < 512) {
        analogWrite(LPWM, motorLeft);
        digitalWrite(RPWM, LOW);
    } else if (steerValueJoystick == 512) {
        analogWrite(RPWM, 0);
        analogWrite(LPWM, 0);
    } else {
        analogWrite(RPWM, motorRight);
        digitalWrite(LPWM, LOW);
    }

    Joystick.setXAxis(steerValueJoystick);
    Joystick.sendState();

    if (button1State != lastButton1State) {
        Joystick.setButton(0, button1State == LOW);
        lastButton1State = button1State;
    }
    if (button2State != lastButton2State) {
        Joystick.setButton(1, button2State == LOW);
        lastButton2State = button2State;
    }
    if (button3State != lastButton3State) {
        Joystick.setButton(2, button3State == LOW);
        lastButton3State = button3State;
    }

    

    delay(5);
}