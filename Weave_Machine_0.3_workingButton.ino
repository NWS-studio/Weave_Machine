#include <AccelStepper.h>

#define dirPin 4
#define stepPin 3
#define buttonPin 2
#define switchPin 8
#define enablePin 9
#define motorInterfaceType 1 // 1 = Driver (step/dir)

AccelStepper stepper(motorInterfaceType, stepPin, dirPin);

const int stepsPerRevolution = 200;
const int microsteps = 8;
const int totalMicrostepsPerRev = stepsPerRevolution * microsteps;

const float gearRatio = 2.0; // Motor turns 2x per 1 output revolution
const float partRotationTurns = 1.0; // 360° = 1 full turn
const int moveSteps = totalMicrostepsPerRev * partRotationTurns;

bool cycleRequested = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(enablePin, OUTPUT);

  digitalWrite(enablePin, !digitalRead(switchPin)); // Enable/disable based on switch state

  stepper.setMaxSpeed(10000);       // Max speed in steps/sec
  stepper.setAcceleration(10000);   // Acceleration in steps/sec^2
  stepper.setCurrentPosition(0);
}

void loop() {
  // Enable/disable motor driver based on switch state (LOW = ENABLED)
  bool motorEnabled = digitalRead(switchPin) == LOW;
  digitalWrite(enablePin, !motorEnabled); // LOW enables motor

  // Only allow cycle if motor is enabled
  if (motorEnabled) {
    static bool lastButtonState = HIGH;
    bool currentButtonState = digitalRead(buttonPin);

    // On button press, trigger one 360° rotation
    if (lastButtonState == HIGH && currentButtonState == LOW) {
      if (!cycleRequested && stepper.distanceToGo() == 0) {
        cycleRequested = true;
        stepper.moveTo(stepper.currentPosition() + moveSteps);
      }
    }
    lastButtonState = currentButtonState;

    // Reset request once move is complete
    if (cycleRequested && stepper.distanceToGo() == 0) {
      cycleRequested = false;
    }

    stepper.run();
  }
}
