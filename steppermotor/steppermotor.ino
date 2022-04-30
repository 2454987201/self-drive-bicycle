#include <stepper1.h>
const float ROTORSTEPS=64;
const float GEAR=64;
const float STEPS=ROTORSTEPS*GEAR;
int StepRequired;
Stepper steppermotor(STEPS,8,9,10,11);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  steppermotor.setSpeed(10);
  //StepRequired=4;
  steppermotor.step(STEPS);
}
