int dir_pin = 6;
int step_pin = 5;
int steps = 200; //fixed number of steps per revolution for stepper motors
int uStep = 1; //revolution, should be one 
int numTurns = 12; //number of turns per revolution
int delayInc = 750; //determines the speed of the revolution
int MS1 = 10; //not connected
int MS2 = 11;  //not connected
int MS3 = 12;  //not connected
int home_switch = 9; //end stopper pin connected to arduino
int sleep_pin = 8; //making this low puts the driver to sleep, making it high awakens the driver for it to run
int steps_home;

int direction;

boolean  currentState = false; //need to define the state before the loop begins in order to understand the starting point of the loop
int robotPin = 2; //pin in arduino from optocouplet connected to the robot

void setup() { //set the pinModes to output
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  pinMode(step_pin, OUTPUT);
  pinMode(sleep_pin, OUTPUT);
  pinMode(robotPin, INPUT);
  pinMode(home_switch, INPUT_PULLUP);

  currentState = digitalRead(robotPin);

  digitalWrite(sleep_pin, HIGH);  // Wake up stepperDriver
  delay(5);  // Wait for Driver wake up
  Serial.begin(9600);

  /* LOW LOW = Full Step //
    // HIGH LOW = Half Step //
    // LOW HIGH = A quarter of Step //
    // HIGH HIGH = An eighth of Step //
  */
  digitalWrite(MS1, LOW);      // Configures to Full Steps
  digitalWrite(MS2, LOW);    // Configures to Full Steps
  digitalWrite(MS3, LOW);     // Configures to Full Steps
  // Start Homing procedure of Stepper Motor at startup:

  while (digitalRead(home_switch)) {  // Do this until the switch is activated
    digitalWrite(dir_pin, LOW);      // (HIGH = anti-clockwise / LOW = clockwise)
    digitalWrite(step_pin, LOW);
    delay(5);                       // Delay to slow down speed of Stepper
    digitalWrite(step_pin, HIGH);
    delay(5);
  }

  while (!digitalRead(home_switch)) { // Do this until the switch is not activated
    digitalWrite(dir_pin, HIGH);
    digitalWrite(step_pin, HIGH);
    delay(5);                       // More delay to slow even more while moving away from switch
    digitalWrite(step_pin, LOW);
    delay(5);
  }

  steps_home = 0; // Reset position variable to zero

  Serial.println("Homing done");

}
void loop() {
  boolean robotInput = !digitalRead(robotPin);  //0 FALSE (BACKWARD), 1 TRUE (FORWARD)
  Serial.println(digitalRead(robotPin));

  if (robotInput && currentState == true) {
    digitalWrite(dir_pin, HIGH);
    for (long i = 0; i < steps * uStep * numTurns; i++) {
      digitalWrite(step_pin, HIGH);
      delayMicroseconds(delayInc * 2);
      digitalWrite(step_pin, LOW);
      delayMicroseconds(delayInc * 2);
    } // motor loop
    currentState = false;
  }

  if (robotInput == 0 && currentState == false) {
    digitalWrite(dir_pin, LOW);

    for (long i = 0; i < steps * uStep * numTurns; i++) { //
      digitalWrite(step_pin, HIGH);
      delayMicroseconds(delayInc * 2);
      digitalWrite(step_pin, LOW);
      delayMicroseconds(delayInc * 2);
    }

    // motor loop
    currentState = true;
  }
}
