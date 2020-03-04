#pragma config(Sensor, dgtl1,  button1,        sensorTouch)
#pragma config(Sensor, dgtl2,  button2,        sensorTouch)
#pragma config(Motor,  port1,           motor1,        tmotorVex393_HBridge, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port10,           motor2,        tmotorVex393_HBridge, openLoop, encoderPort, I2C_2)
#pragma config(Motor, port2, motor3, tmotorVex393_HBridge, openLoop)

/*
******************
Contains 2 main functions, one is currently commented out. movement() makes it move,
object_placement() makes it place the object. Movement times are calibrated, but the placement
times are not.
******************
*/


bool button1_pushed; //flag to store button1 input
bool button2_pushed; //flag to store button2 input

int speed = -30; //Default speed used, sign sets direction
int sp1 = speed;
int sp2 = speed*1.15; //This adjusts for motor quality difference

/* monitorInput()
*
*  Used to flag button inputs
*       - this avoids errors caused by program recognizing input, taking action, and
*         reading input again before button is released
*/
void monitorInput()
{
  if(SensorValue(button1) && !button1_pushed)
  {
    button1_pushed = true;
  }

  if(SensorValue(button2) && !button2_pushed)
  {
    button2_pushed = true;
  }
}

void movement() // For movement tests
{
	// Define tag for enumerated type
	enum T_movement_state {
		STOP = 0,
		FORWARD = 1,
		TURN = 2
	};

	// Declare variable to hold state, intialize to STOP state.
	T_movement_state movement_state = STOP;

	while(true)
	{
		// This function updates the button1_pushed and button2_pushed flags. Set to false
		// immediately to prevent double calls
		button1_pushed = button2_pushed = false;
		monitorInput();

		// Switch the states.
		switch(movement_state) {

		case STOP:
			// Turn motor off.
			motor[motor1] = 0;
			motor[motor2] = 0;
			if ( button1_pushed ) { // If button 1 pushed, go into FORWARD state
				movement_state = FORWARD;
			}
			if (button2_pushed ) { // If button2 pushed, go into TURN state
				movement_state = TURN;
			}
			break;
		case FORWARD:
			// Makes the robot go forward for roughly 1m
			motor[motor1] = sp1;
			motor[motor2] = sp2;
			wait1Msec(3700);
			movement_state = STOP;
			break;
		case TURN:
			// Makes the robot turn to the right roughly 90 degrees
			motor[motor1] = sp1;
			motor[motor2] = -sp2;
			wait1Msec(750);
			movement_state = STOP;
		default:
		}

	}//end while
}

void object_placement() // For object placement tests
{
	enum T_movement_state {
		STOP = 0,
		PLACE = 1
	};

	// Declare variable to hold state, intialize to STOP state.
	T_movement_state movement_state = STOP;

	while(true)
	{
		// This function updates the button1_pushed and button2_pushed flags.
		button1_pushed = false;
		monitorInput();

		// Switch the states.
		switch(movement_state) {

		case STOP:
			// Turn motor off.
			motor[motor3] = 0;
			if ( button1_pushed ) {
				// If button1 pushed, change to the MOTOR_RUNNING state.
				movement_state = PLACE;
				// Clear flag to indicate button 1 processed.
				button1_pushed = false;
			}
			break;
		case PLACE:
			// Run the motor slowly to push the object off.
			motor[motor3] = 10;
			wait1Msec(1500);
			movement_state = STOP;
			break;
		default:
		}

	}//end while
}

task main()
{
	button1_pushed = button2_pushed = false;

	movement();
	//object_placement();

}// end main
