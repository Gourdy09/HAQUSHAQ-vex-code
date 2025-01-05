/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Om Patel                                                  */
/*    Created:      1/5/2025, 10:23:23 AM                                     */
/*    Description:  V5 project for HAQUSHAQ                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    CONTROLS:                                                               */
/*    RIGHT TRIGGER: move forwards         LEFT TRIGGER: move backwards       */
/*    LEFT JOYSTICK (Axis 4): Turn Left/Right                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain Brain;

// Define your global instances of motors and other devices here

// Drivetrain motors
vex::motor front_left_motor = vex::motor(vex::PORT1);  // Motor connected to port 1
vex::motor front_right_motor = vex::motor(vex::PORT10, true);  // Motor connected to port 10 (inverted)
vex::motor back_left_motor = vex::motor(vex::PORT11);  // Motor connected to port 11
vex::motor back_right_motor = vex::motor(vex::PORT20, true);  // Motor connected to port 20 (inverted)
vex::controller Controller1 = vex::controller(vex::controllerType::primary);  // Primary controller

// Function to move the robot
void move(int turnValue, vex::directionType moveDirection) {
    front_left_motor.spin(moveDirection, turnValue + 100, vex::percent);  // Turn based on left joystick horizontal
    back_left_motor.spin(moveDirection, turnValue + 100, vex::percent);   // Same for back left motor

    front_right_motor.spin(moveDirection, -turnValue + 100, vex::percent);  // Opposite turn for right motors
    back_right_motor.spin(moveDirection, -turnValue + 100, vex::percent);   // Same for back right motor
}

// Function to stop the robot
void stop() {
    front_left_motor.stop();
    front_right_motor.stop();
    back_left_motor.stop();
    back_right_motor.stop();
}

int main() {
    // Boot Message
    Brain.Screen.printAt(10, 50, "HAQUSHAQ IS SO SKIBIDI");
    
    // Infinite loop to continuously read controller inputs
    while (true) {

        // Check if the right trigger is pressed
        int rightTrigger = Controller1.ButtonR2.pressing();
        int leftTrigger = Controller1.ButtonL2.pressing();

        // Get the position of the left joystick for turning
        int turnValue = Controller1.Axis4.position(vex::percent);

        // Set the move direction to correspond with which trigger is pressed
        vex::directionType moveDirection = rightTrigger ? vex::directionType::fwd : leftTrigger ? vex::directionType::rev : vex::directionType::undefined;

        if (moveDirection != vex::directionType::undefined) {
            move(turnValue, moveDirection);
        }
        else {
            stop();  // Stop robot if no input is given
        }

        sleep(20);  // Delay to make sure the robot responds to the controller smoothly
    }
}
