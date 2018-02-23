#include <Password.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#define pir 10
#define smokeA0 A5
#define sire 12
int out = 13;
int flag = 0;
int p1 = 0;
int smoke = 0;
int wait = 30;
int sensorThres = 400;
float sensorVals[] = {0, 0};
Password password = Password( "1234" );
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = { 9, 8, 7, 6 }; // Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte colPins[COLS] = { 5, 4, 3, 2,}; // Connect keypad COL0, COL1 and COL2 to these Arduino pins.


// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  pinMode(out, OUTPUT);
  pinMode(pir, INPUT);
  pinMode(sire, OUTPUT);
  pinMode(smokeA0, INPUT);
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
  delay(1000 * 30);
  Serial.begin(9600);
}

void loop() {
  p1 = digitalRead(pir);
  smoke = analogRead(smokeA0);
  sensorVals[0] = digitalRead(pir);
  sensorVals[1] = analogRead(smokeA0);
  if (p1 == HIGH || smoke > sensorThres) {
    if (wait == 30) {
      while (wait > 0) {
        digitalWrite(out, HIGH);
        delay(100);
        digitalWrite(out, LOW);
        delay(100);
        keypad.getKey();
        wait--;
      }
      Serial.print(sensorVals[0]);
      Serial.print(",");
      Serial.println(sensorVals[1]);
      siren();
    }
  }

  keypad.getKey();
}

//take care of some special events
void keypadEvent(KeypadEvent eKey) {
  switch (keypad.getState()) {
    case PRESSED:
      switch (eKey) {
        case '#': checkPassword(); break;
        case '*': password.reset(); break;
        default: password.append(eKey);
      }
  }
}

void checkPassword() {
  if (password.evaluate()) {
    Serial.println("Success");
    flag = 1;
    sirenoff();
    sysoff();
    password.reset();
  } else {

    digitalWrite(out, HIGH);
    delay(1000 * 5);
    digitalWrite(out, LOW);
  }
}

void sysoff() {
  while (flag == 1) {
    digitalWrite(out, HIGH);
    delay(100);
    digitalWrite(out, LOW);
    delay(100);
    char key = keypad.getKey();
    if (key)
    {
      switch (key)
      {
        case 'A':
          flag = 0;
          wait = 30;
          delay(1000 * 15);
          break;
        case 'B': siren();
          break;
      }
    }
  }
}

void siren()
{
  digitalWrite(sire, HIGH);
  wait = 0;
}

void sirenoff() {
  digitalWrite(sire, LOW);
}

