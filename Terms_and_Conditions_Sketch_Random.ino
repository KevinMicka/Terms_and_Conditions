#include <AccelStepper.h>
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  3     // IN1 on the ULN2003 driver 1
#define motorPin2  4     // IN2 on the ULN2003 driver 1
#define motorPin3  5     // IN3 on the ULN2003 driver 1
#define motorPin4  6     // IN4 on the ULN2003 driver 1

#define SWING 500
#define MINSPEED 50
#define MAXSPEED 1000
#define RANDOMNESS 1023 // setbetween 0 (not random) and 1023

#define pluckPin   A0    // Connect to pickup
// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
  delay(8000); // delay 8 sec (8000ms)
  pinMode(A1, INPUT);
  randomSeed(analogRead(5));
  
  stepper1.setMaxSpeed(1000.0);
  //stepper1.setAcceleration(100.0);
  stepper1.setSpeed(200);
  //stepper1.moveTo(800);

/// FIND STRING ///
/// Attempt to find the string by listening to the pickup
/// on pluckPin (A0)
  //stepper1.run();
  int  val = 0; //don't touch
  int  threshold = 11;  // 5mv * threshold = trigger voltage
                        // set between 0 and 1023 
                        // high enough to not get noise, 138 for preamp
                        // low enough for pickup, 11- 90 for passive
  while (val < threshold) {
    val = analogRead(pluckPin); 
    stepper1.runSpeed(); 
  }

  stepper1.setCurrentPosition  (0); 
/// END FIND STRING ///

/// INITIALIZE PLUCKER ///
  stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(100.0);
  stepper1.setSpeed(200);
  stepper1.moveTo(SWING);
/// END INITIALIZE PLUCKER ///

}//--(end setup )---

void loop() {

   
/// MAIN  
  //Change direction when the stepper reaches the target position
  if (stepper1.distanceToGo() == 0) {
    float randomness = RANDOMNESS; 
//    float randomness = analogRead(1); // Uncomment to use pot reading
    float rand_val = pow(map(randomness, 0, 1024, 1, 10), (random(-1000, 1000) / 1000.0));
    stepper1.setAcceleration(map(rand_val, 0, 10, MINSPEED, MAXSPEED));
    stepper1.moveTo(-stepper1.currentPosition());
  }
  stepper1.run();
}
