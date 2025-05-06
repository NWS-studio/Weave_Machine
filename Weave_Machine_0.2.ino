#include <AccelStepper.h>

#define dirPin 4
#define stepPin 3
#define buttonPin 2
#define motorInterfaceType 1 // 1 = Driver (step/dir)

AccelStepper stepper(motorInterfaceType, stepPin, dirPin);

const int stepsPerRevolution = 200;
const int microsteps = 8;
const int totalMicrostepsPerRev = stepsPerRevolution * microsteps;

const float gearRatio = 2.0; // Motor turns 2x per 1 output revolution
const float partRotationTurns = 1.0; // 360° = 1 full turn
const int moveSteps = totalMicrostepsPerRev * partRotationTurns;

bool movingForward = true;
bool pauseState = false;
unsigned long pauseStartTime = 0;
bool waitingForPause = false;

void setup() {
  stepper.setMaxSpeed(5000);       // Max speed in steps/sec
  stepper.setAcceleration(4000);   // Acceleration in steps/sec^2
  stepper.setCurrentPosition(0);
  stepper.moveTo(moveSteps);
}

void loop() {
  if (!pauseState && stepper.distanceToGo() == 0 && !waitingForPause) {
    pauseStartTime = millis();
    pauseState = true;
    waitingForPause = true;
  }

  if (pauseState) {
    if (millis() - pauseStartTime >= 2000) {
      movingForward = !movingForward;
      stepper.moveTo(movingForward ? moveSteps : -moveSteps);
      pauseState = false;
      waitingForPause = false;
    }
  }

  stepper.run();
}
