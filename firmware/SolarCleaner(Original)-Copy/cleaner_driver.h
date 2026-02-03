#pragma once

#ifndef CLEANER_DRIVER_H
#define CLEANER_DRIVER_H

#define CW 1	//rotates motor Clockwise
#define CCW 0	//rotates motor Counter-Clockwise
#define debug 0 // change it to 1 to enable debugging in Serial Monitor 

class CleanerDriver {
public:
	void Init();
	void forward(int speed);
	void backward(int speed);
	void segment_right(int speed);
	void segment_left(int speed);
	void stop();
	void hor_stop();
	void for_stop();


};

#endif // !CLEANER_DRIVER_H


