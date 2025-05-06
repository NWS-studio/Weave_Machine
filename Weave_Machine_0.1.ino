#include <AccelStepper.h>

// Define connections and motor interface type
#define dirPin 4
#define stepPin 3
#define motorInterfaceType 1 // 1 = Driver (step/dir)

AccelStepper stepper(motorInterfaceType, stepPin, dirPin);

// Motor and gear characteristics
const int stepsPerRevolution = 200; // 200 full steps
const int microsteps = 16;           // 16 microsteps
const int totalMicrostepsPerRev = stepsPerRevolution * microsteps;

// Gear ratio: 1 rotation of worm = 1/35 of output gear
// 180° = 0.5 rev => worm must rotate 0.5 * 35 = 17.5 turns
const float gearRotationTurns = 17.5;
const int moveSteps = totalMicrostepsPerRev * gearRotationTurns;

// States
bool movingForward = true;
bool pauseState = false;
unsigned long pauseStartTime = 0;

void setup() {
  stepper.setMaxSpeed(2000);    // Adjust as needed
  stepper.setAcceleration(500); // Smooth acceleration
}

void loop() {
  if (!pauseState) {
    if (stepper.distanceToGo() == 0) {
      if (movingForward) {
        stepper.move(moveSteps);
      } else {
        stepper.move(-moveSteps);
      }
      movingForward = !movingForward; // Switch direction next time
      pauseState = true;
      pauseStartTime = millis();
    }
  } else {
    if (millis() - pauseStartTime >= 1000) { // 1 second pause
      pauseState = false;
    }
  }

  stepper.run();
}
