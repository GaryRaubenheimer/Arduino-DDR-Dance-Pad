/*
   DDR pad programming

   check if any button is pressed [a button's state has changed]
   for more than 50ms     -debounce
   take that as an input
   if button state goes from low to high then press keyboard output
   else if button state goes from high to low then release keyboard output

*/
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'

#include "Keyboard.h"

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long debounceDelay = 20;    // the debounce time; increase if the output flickers

int ledState = LOW;

// button class
class button {
    int buttonPin;
    int buttonState;
    int lastButtonState = LOW;
    unsigned long lastDebounceTime = 0;
    int reading;
    int lastReading = LOW;
    char KEY;

  public:
    void setPin(char pin, char key) {  //sets pins as pullup
      KEY = key;
      buttonPin = pin;
      pinMode(buttonPin, INPUT_PULLUP);
    }

    void Reading() {
      reading = digitalRead(buttonPin);

    }

    void stateChange() {
      // If the button changed, due to noise or pressing:
      if (reading != lastReading) {
        // reset the debouncing timer
        lastDebounceTime = millis();
      }

      if (((millis() - lastDebounceTime) > debounceDelay)) {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:
        // if button state has changed:

        if (reading != lastButtonState) {

          // -OUTPUT
          buttonState = reading;
          Serial.println(lastButtonState);
          if (buttonState == HIGH) {
            Keyboard.press(KEY);
            ledState = HIGH;
            lastButtonState =HIGH;
          }
          if (buttonState == LOW) {
            Keyboard.release(KEY);
            ledState = LOW;
            lastButtonState = LOW;
          }
        }
        // save the reading. Next time through the loop, it'll be the lastButtonState:

      }
      lastReading = reading;
    }
};

// constants won't change. They're used here to set pin numbers:
button up;
button left;
button down;
button right;

const int ledPin = 13;

void setup() {
  up.setPin(8, UP);
  left.setPin(4, LEFT);
  down.setPin(2, DOWN);
  right.setPin(7, RIGHT);
  pinMode(ledPin, OUTPUT);
  Keyboard.begin();
  Serial.begin(9600);

  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  // read the state of the buttons into a local variable:
  up.Reading();
  left.Reading();
  down.Reading();
  right.Reading();

  up.stateChange();
  left.stateChange();
  down.stateChange();
  right.stateChange();

  // set the LED:
  digitalWrite(ledPin, ledState);
}

