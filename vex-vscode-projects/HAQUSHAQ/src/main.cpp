// Include necessary headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "vex.h"

using namespace vex;

// Define brain instance
brain Brain;

// Define motor ports and instances
vex::motor front_left_motor = vex::motor(vex::PORT17, true);  // Motor connected to port 17
vex::motor front_right_motor = vex::motor(vex::PORT16);  // Motor connected to port 16
vex::motor back_left_motor = vex::motor(vex::PORT14, true);  // Motor connected to port 14
vex::motor back_right_motor = vex::motor(vex::PORT15);  // Motor connected to port 15

// Mobile stick motors
vex::motor mobile_stick_holder_motor = vex::motor(vex::PORT11, true);  // Motor to hold the goal stick (inverted)

// Other motors for various functions
vex::motor intake_motor = vex::motor(vex::PORT20);  // Motor to suck in the donuts
vex::motor conveyer_motor = vex::motor(vex::PORT1);  // Motor to run the conveyer belt
vex::motor push_climb_motor = vex::motor(vex::PORT12);  // Motor to climb and push down donuts

// Define the controller
vex::controller Controller1 = vex::controller(vex::controllerType::primary);  // Primary controller

// Button Toggles (true = +45°, false = -45°)
static bool mobileStickToggleState = true;  // Initially at +45 degrees
static bool pushClimbToggleState = true;   // Initially at +45 degrees

// Function to generate and set random seed based on system data
void initializeRandomSeed() {
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  // Combine these values into a single integer seed
  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;
  
  // Set the random seed
  srand(seed);
}

// Function to initialize robot configuration
void vexcodeInit() {
  initializeRandomSeed();  // Initialize random seed
}

// Function to play sound (helper function for cleaner code)
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);  // Wait for sound to play
}

// Function to move the robot (drive forward/backward and rotate)
void move(int turnValue, vex::directionType moveDirection) {
  front_left_motor.spin(moveDirection, turnValue + 100, percent);  // Left motors
  back_left_motor.spin(moveDirection, turnValue + 100, percent);

  front_right_motor.spin(moveDirection, -turnValue + 100, percent);  // Right motors (opposite direction)
  back_right_motor.spin(moveDirection, -turnValue + 100, percent);
}

// Functions to control push/climb mechanism
void push() {
  push_climb_motor.spin(forward);
}

void pull() {
  push_climb_motor.spin(reverse);
}

// Function to toggle mobile stick position
void toggle_mobile_stick() {
  mobile_stick_holder_motor.spin(forward);
  mobile_stick_holder_motor.setVelocity(100, percent);
  mobile_stick_holder_motor.spin(forward);  // Move to +45 degree position
}

void untoggle_mobile_stick() {
  mobile_stick_holder_motor.spin(reverse);
  mobile_stick_holder_motor.setVelocity(100, percent);
  mobile_stick_holder_motor.spin(reverse);  // Move to -45 degree position
}

// Function to toggle the push/climb motor position
void toggle_push_climb() {
  if (pushClimbToggleState) {
    push_climb_motor.spin(reverse);
    push_climb_motor.setVelocity(100, percent);
    push_climb_motor.spinToPosition(-45, degrees);  // Move to -45 degree position
    pushClimbToggleState = false;  // Update state
  } else {
    push_climb_motor.spin(forward);
    push_climb_motor.setVelocity(100, percent);
    push_climb_motor.spinToPosition(60, degrees);  // Move to +45 degree position
    pushClimbToggleState = true;  // Update state
  }
}

// Main function to run the robot
int main() {
  // Boot message
  Brain.Screen.print("HAGUSHAQ IS SO SKIBIDI\nHAGUSHAQ IS SO SKIBIDI");

  // Infinite loop to continuously read controller inputs
  while (true) {
    // Get controller axis positions for various actions
    int intakeValue = Controller1.Axis2.position(vex::percent);  // Intake motor control
    int driveTrainValue = Controller1.Axis3.position(vex::percent);  // Forward/backward control
    int rotateValue = Controller1.Axis1.position(vex::percent);  // Turning control
    
    // Control motors based on joystick inputs
    conveyer_motor.spin(forward, intakeValue, vex::percent);
    intake_motor.spin(forward, intakeValue, vex::percent);

    front_left_motor.spin(forward, driveTrainValue + rotateValue, vex::percent);
    front_right_motor.spin(forward, driveTrainValue - rotateValue, vex::percent);
    back_left_motor.spin(forward, driveTrainValue + rotateValue, vex::percent);
    back_right_motor.spin(forward, driveTrainValue - rotateValue, vex::percent);

    // Handle mobile stick toggle with the B button
    if (Controller1.ButtonB.pressing()) {
      toggle_mobile_stick();  // Toggle mobile stick position
    }
    if (Controller1.ButtonX.pressing()) {
      untoggle_mobile_stick();  // Untoggle mobile stick position
    }

    // Handle push/climb mechanism with the Up and Down buttons
    if (Controller1.ButtonUp.pressing()) {
      push();  // Push/climb
    }
    if (Controller1.ButtonDown.pressing()) {
      pull();  // Pull/climb
    }

    vex::task::sleep(20);  // Delay to ensure smooth controller response
  }
}
