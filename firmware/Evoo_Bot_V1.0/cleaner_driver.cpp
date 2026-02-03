#include "cleaner_driver.h"
#include <RobojaxBTS7960.h>
#include "Globals.h"
#include "Arduino.h"

/************* Self Modified Version of BTS7960 library is used, original is available at Robojax here
http://bit.ly/bts7960-library
*/

/********** Two Motor objects for forward/backward(up/down) Movements*********************/
RobojaxBTS7960 forwardMT1(FOR1_R_EN, FOR1_RPWM, FOR1_L_EN, FOR1_LPWM, debug);
RobojaxBTS7960 forwardMT2(FOR2_R_EN, FOR2_RPWM, FOR2_L_EN, FOR2_LPWM, debug);

/********** Two Motor objects for Horizontal Movements (right/left) *********************/
RobojaxBTS7960 horizontalMT1(HOR1_R_EN, HOR1_RPWM, HOR1_L_EN, HOR1_LPWM, debug);
RobojaxBTS7960 horizontalMT2(HOR2_R_EN, HOR2_RPWM, HOR2_L_EN, HOR2_LPWM, debug);


void CleanerDriver::Init() {
	forwardMT1.begin();
	forwardMT2.begin();
	horizontalMT1.begin();
	horizontalMT2.begin();
	/* sets Brush Motor relay pin as OUTPUT*/
	pinMode(BR_MT, OUTPUT);
	// turn off Brush Motor initially(active LOW)
	digitalWrite(BR_MT, HIGH);

}

/*************runs the cleaner forward along with brush motor********
*- speed: 0-100
*/
void CleanerDriver::forward(int speed) {
	forwardMT1.rotate(speed, CW);
	forwardMT2.rotate(speed, CW);
	// turn on the Brush Motor
	digitalWrite(BR_MT, LOW); // change wires of Brush motor in relay connections, if direction is reversed;
	horizontalMT1.stop();
	horizontalMT2.stop();
}

void CleanerDriver::backward(int speed) {
	forwardMT1.rotate(speed, CCW);
	forwardMT2.rotate(speed, CCW);
	// turn on the Brush Motor
	digitalWrite(BR_MT, LOW); // change wires of Brush motor in relay connections, if direction is reversed;
	horizontalMT1.stop();
	horizontalMT2.stop();
}

/******* runs the cleaner horizontally on the solar panel using Segment to Segment movement
*-SEGMENT_DELAY: calculate this delay to move cleaner horizontally for a moment
*- stops all motors during Segment to Segment movement except horizontal motors
*- cleaner movement from right to left is considered DRY CLEANING
*/
void CleanerDriver::segment_left(int speed) {
	forwardMT1.stop();
	forwardMT2.stop();
	// turn off the Brush Motor
	digitalWrite(BR_MT, HIGH);
	horizontalMT1.rotate(speed, CCW);
	horizontalMT2.rotate(speed, CCW);
	delay(SEGMENT_DELAY); //calculated delay(by trial and error) for the segment to segment movement
	horizontalMT1.stop();
	horizontalMT2.stop();
}

/******* runs the cleaner horizontally on the solar panel using Segment to Segment movement
*-SEGMENT_DELAY: calculate this delay to move cleaner horizontally for a moment
*- cleaner movement from left to right is considered WET CLEANING
*/
void CleanerDriver::segment_right(int speed) {
	forwardMT1.stop();
	forwardMT2.stop();
	// turn off the Brush Motor
	digitalWrite(BR_MT, HIGH);
	horizontalMT1.rotate(speed, CW);
	horizontalMT2.rotate(speed, CW);
	delay(SEGMENT_DELAY); //calculated delay(by trial and error) for the segment to segment movement
	horizontalMT1.stop();
	horizontalMT2.stop();
}


/************ stops the whole cleaner i.e. all motors stops  *******************/
void CleanerDriver::stop() {
	forwardMT1.stop();
	forwardMT2.stop();
	horizontalMT1.stop();
	horizontalMT2.stop();
}

/******** only stops the forward/backward motors****/
void CleanerDriver::for_stop() {
	forwardMT1.stop();
	forwardMT2.stop();
}

/******** only stops the horizontal motors****/
void CleanerDriver::hor_stop() {
	horizontalMT1.stop();
	horizontalMT2.stop();
}