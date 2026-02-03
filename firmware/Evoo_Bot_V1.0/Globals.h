#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#define EN_DIGITAL_R_SEN 
//#define EN_ANALOG_R_SEN

#define BLYNK_PRINT Serial

#define TRUE 1
#define FALSE 0

#ifdef EN_ANALOG_R_SEN

#define R_SEN   A0

#endif // EN_ANALOG_R_SEN


#ifdef EN_DIGITAL_R_SEN

#define R_SEN	12	//connect Jumper if Digital Input Rain Sensor is used.

#endif // EN_DIGITAL_R_SEN

// For 3 Pin Water Level Sensor(only use WL_SEN_S for 2 Pin)
#define WL_SEN_S  41
#define WL_SEN_EN 38

#define W_PUMP  A1
#define BR_MT	A2

#define SONAR_NUM 3      // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define FOR_SPEED_MT	60	// forward/backward movement speed (0 to 100), default is 60 
#define HOR_SPEED_MT	60	// horizontal movement speed (0 to 100), default is 60
#define SEGMENT_DELAY	1200 // in ms, calculate this delay by Trial and Error for perfect Segment to Segment movement

#define FOR_TRIG  48  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define FOR_ECH0     49  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define BACK_TRIG  46  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define BACK_ECH0     47  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define GUARD_TRIG  44  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define GUARD_ECH0     45  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define HOR1_LPWM   9
#define HOR1_RPWM   8

#define HOR2_LPWM   7
#define HOR2_RPWM   6

#define FOR1_LPWM	3
#define FOR1_RPWM	2

#define FOR2_LPWM   5
#define FOR2_RPWM	4

#define HOR1_L_EN   37
#define HOR1_R_EN   40

#define HOR2_L_EN   35
#define HOR2_R_EN	36

#define FOR1_L_EN	33
#define FOR1_R_EN	34

#define FOR2_L_EN	31
#define FOR2_R_EN	32

#define LCD_RS  27
#define LCD_EN  26
#define LCD_D4  25
#define LCD_D5  24
#define LCD_D6  23
#define LCD_D7  22

#define PHONE_NO                      "\"+ZZxxxxxxxxxx\""  // insert your phone number with country code (e.g. +92...) 
#define WARN_RAINING                  "Evoo-Bot 2021 V1.0\n It is raining, please turn off the whole system for safety"
#define WARN_WATER_LEVEL              "Evoo-Bot 2021 V1.0\n Water Tank is empty, please fill the tank for Wet Cleaning"
#define WARN_CLEANER_RUN              "Evoo-Bot 2021 V1.0\n Cleaner is running"
#define WARN_CLEANER_RUN_ALARM_TRIG   "Evoo-Bot 2021 V1.0\n Cleaner is Turned ON after Alarm Trigger"
#define WARN_CLEANER_REST             "Evoo-Bot 2021 V1.0\n Cleaner is resting under the Cleaner Guard"



#endif
