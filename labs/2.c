#pragma config(Sensor, in1,    motor_angle,    sensorPotentiometer)
#pragma config(Motor,  port1,           motor1,        tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**********************************************************
** bool set_motor( int target )
** Runs motor to try and hit target angle
** Inputs:
**   target  - target reading from potentiometer.
** Output:
**    true   - motor has reached desired position
**    false  - motor has not hit target position
**
** Sets motor output.
** Reads motor from port motor_angle
**********************************************************/
bool set_motor( int target )
{
	// Compute difference from current position and target position.
	int delta = target - SensorValue[motor_angle];
	//Initiate maximum speed value and accuracy value
	int max = 80;
	int acc = 10;

	//Determine if delta is more than max speed, set delta to max
	if ( delta > max ) {
		delta = max;
  } else if ( delta < -max ) { //If delta is less than negative max speed, set delta to negative max
		delta = -max;
  }
  //Check if delta is between the the accuracy window, if yes, stop motor, if not, set motor to delta speed
	if (delta<acc && delta>-acc){
		motor[motor1] = 0;
	} else {
		motor[motor1] = delta;
	}
	return(false); //Return false because boolean function, boolean is never read though
}



task main()
{
	// Target value to move motor towards.
	int target_value = 1500;

	// Time unitl motor hits target value.
	int first_time;
	// Maximum error after first time motor hits target value.
	int max_error;
	// Noted if motor hit target value.
	bool hit_target = false;

	int delta;

	// Reset timer.
	clearTimer(T1);

	// Run for either 5 seconds or until we get to target value
	while( time1(T1) < 5000 ) {

	  // Implements one iteration of control algorithm.
		set_motor( target_value );

		// If we haven't hit target value yet...
		if ( ! hit_target ) {
			// Check if we encounter target_value again.
			if ( SensorValue[motor_angle] == target_value ) {
				// Note hit target value.
				hit_target = true;

				// Record first time.
				first_time = time1(T1);

				// Reset maximum error value.
				max_error = 0;
			}
		} else {

		  // Find deviation from target value.
			delta = SensorValue[motor_angle] - target_value;

			// Find absolute value of deviation.
			if ( delta < 0 ) {
				delta = -1 * delta;
			}

			// If deviation is larger then previously found maximum deviation, update maximum deviation.
			if ( delta > max_error ) {
				max_error = delta;
			}

		} // if (! hit_target) else
	}  // match while( time1(T1) < 5000 )

	// Turn off motor
	motor[motor1] = 0;

	// Record results.
	if ( hit_target ) {
		writeDebugStream("First Time: %d\tMax Error: %d\n",first_time,max_error);
		} else {
		writeDebugStream("No convergence.\n");
	}

	// Infinite loop to keep DebugStream open so output can be read.
	while(1){
		//writeDebugStream("%d\n", SensorValue[motor_angle]);
}
}

