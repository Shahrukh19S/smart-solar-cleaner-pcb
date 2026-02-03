/*
	Name:       Evoo_Bot_V1.0.ino
	Created:	4/24/2021 8:23:33 PM
	Author:     Abdullah Ansari
*/

#define BLYNK_PRINT Serial

#include "Globals.h"
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <LiquidCrystal.h>
#include "support_rtc.h"
#include "support_func.h"
#include "cleaner_driver.h"

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
// Option: use Config.h (copy from Config.example.h) and #include "Config.h" instead of defining here.
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";

// Your WiFi credentials (for ESP-01). Set password to "" for open networks.
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600	//Don't forget to permanently change the default baud rate of ESP fromm 115200 to 9600 using AT							  //command

//LCD Pins setup
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

ESP8266 wifi(&EspSerial);

uint8_t curr_HOUR = 00;
uint8_t curr_MIN = 00;
uint8_t curr_SEC = 00;


uint8_t ALARM_HOUR = 00;
uint8_t ALARM_MIN = 00;
uint8_t ALARM_SEC = 00;

/************************ The reference of this two variables is given to saveAlarmTime() function in order to save the Alarm Times in support_rtc.cpp ********************************
  *- We must keep this variable references valid until the Alarm is not set on RTC, therefore, they're defined here(not local variables)
*/
char on_time_str[20];
char off_time_str[20];

boolean isFreeA1 = true;
boolean isFreeA2 = true;

uint8_t total_segments;
uint8_t current_segment;
boolean cleaner_allow_back = true;



CleanerDriver cleaner;

BLYNK_WRITE(V1) // Start Button Widget writes to Virtual Pin V1
{
	if (param.asInt() == 1) {     // if Button sends 1
		Serial.println("Cleaner is turned ON");
		SendSms(PHONE_NO, WARN_CLEANER_RUN);
		DryCleaning();
		WetCleaning();

	}
}

/* This method is called automatically when the user selects the start time on the time input widget
	*- Remember- even though you change nothing(start, timezone, days) and press OK button in the time input widget, this method will be called with the current start, timezone and days.
*/

BLYNK_WRITE(V2) {
	TimeInputParam t(param);

	// Process start time

	if (t.hasStartTime())
	{
		//t.getStartHour = Alarm Time in Hours
		// ALARM_HOUR, ALARM_MINUTE is the Elaps Time after subtraction, like 00:02 (2 minutes Alarm)
		ALARM_HOUR = (t.getStartHour() > curr_HOUR) ? t.getStartHour() - curr_HOUR : curr_HOUR - t.getStartHour();
		//t.getStartMinute =  Alarm Time in Minutes.
		ALARM_MIN = (t.getStartMinute() > curr_MIN) ? t.getStartMinute() - curr_MIN : curr_MIN - t.getStartMinute();
		ALARM_SEC = (t.getStartSecond() > curr_SEC) ? t.getStartSecond() - curr_SEC : curr_SEC - t.getStartSecond();

		/******************************************************************************************************
		 if alarm_time_str[20] were used it will create two references, one here and the second one in t.hasStopTime() even though the name is same.
		 and since we couldn't keep the references (1st reference would be destroyed when the second alarm_time_str is created since the name is same )
		 we would've garbage values in support_rtc.cpp. therefore, two variables (on_time_str and off_time_str were created globally)
		 *****************************************************************************************************/
		 //char alarm_time_str[20];

		sprintf(on_time_str, "%02d:%02d:%02d", ALARM_HOUR, ALARM_MIN, ALARM_SEC);

		Serial.print("Start Log is =");
		Serial.println(on_time_str);

		//save the ON Time in support_rtc.cpp
		RTC::saveAlarmTime(1, on_time_str);

		Serial.println(String("Start: ") +
			t.getStartHour() + ":" +
			t.getStartMinute() + ":" +
			t.getStartSecond());
	}
	else if (t.isStartSunrise())
	{
		Serial.println("Start at sunrise");
	}
	else if (t.isStartSunset())
	{
		Serial.println("Start at sunset");
	}
	else
	{
		// Do nothing
	}

	// Process timezone
	// Timezone is already added to start/stop time

	Serial.println(String("Time zone: ") + t.getTZ());

	// Get timezone offset (in seconds)
	Serial.println(String("Time zone offset: ") + t.getTZ_Offset());

	// Process weekdays (1=Mon, 2=Tue, 3=Wed, ...)

	//for (int i = 1; i <= 7; i++) {
	//	if (t.isWeekdaySelected(i)) {
	//		Serial.println(String("Day ") + i + " is selected");
	//	}
	//}

	Serial.println();
}


/**************************************************************************************************
This method is called when the user selects the Stop Time for the cleaner on the Time input Widget.
	1: This method can set the alarm time on RTC but when its triggered it doesn't do anything. Since, for stopping the		 cleaner we must stop it under the cleaner Guard.
***************************************************************************************************/
/*
BLYNK_WRITE(V3) {
	TimeInputParam t(param);

	// Process stop time

	if (t.hasStopTime())
	{
		//t.getStopHour = Alarm Time in Hours
		// ALARM_HOUR, ALARM_MINUTE is the Elaps Time after subtraction, like 00:02 (2 minutes Alarm)
		ALARM_HOUR = (t.getStopHour() > curr_HOUR) ? t.getStopHour() - curr_HOUR : curr_HOUR - t.getStopHour();
		//t.getStartMinute =  Alarm Time in Minutes.
		ALARM_MIN = (t.getStopMinute() > curr_MIN) ? t.getStopMinute() - curr_MIN : curr_MIN - t.getStopMinute();
		ALARM_SEC = (t.getStopSecond() > curr_SEC) ? t.getStopSecond() - curr_SEC : curr_SEC - t.getStopSecond();

		sprintf(off_time_str, "%02d:%02d:%02d", ALARM_HOUR, ALARM_MIN, ALARM_SEC);

		Serial.print("Stop Log is =");
		Serial.println(off_time_str);

		//save the OFF Time in support_rtc.cpp
		RTC::saveAlarmTime(2, off_time_str);

		Serial.println(String("Stop: ") +
			t.getStopHour() + ":" +
			t.getStopMinute() + ":" +
			t.getStopSecond());
	}
	else if (t.isStopSunrise())
	{
		Serial.println("Start at sunrise");
	}
	else if (t.isStopSunset())
	{
		Serial.println("Start at sunset");
	}
	else
	{
		// Do nothing
	}

	// Process timezone
	// Timezone is already added to start/stop time

	Serial.println(String("Time zone: ") + t.getTZ());

	// Get timezone offset (in seconds)
	Serial.println(String("Time zone offset: ") + t.getTZ_Offset());

	// Process weekdays (1. Mon, 2. Tue, 3. Wed, ...)

	//for (int i = 1; i <= 7; i++) {
	//	if (t.isWeekdaySelected(i)) {
	//		Serial.println(String("Day ") + i + " is selected");
	//	}
	//}

	Serial.println();
}
*/


/************Gets the current time to calculate exact Alarm time.
	*- e.g. if 12:05 is the Alarm Time and current time is 12:00, then exact Alarm time is 00:05 (5 minutes)
	*- this 00:05 alarm time is with respect to 00:00 Time which is the default time set on RTC when the hardware started
	*- So, whatever the current time is on RTC, we just have to add 00:05 (5 minutes) to the current time on RTC.

	NOTE: YOU MUST SET THE CURRENT TIME FIRST BEFORE SETTING THE ALARM TIME FOR START/STOP CLEANER.
*/
BLYNK_WRITE(V3) {
	TimeInputParam t(param);

	// Process start time

	if (t.hasStartTime())
	{
		curr_HOUR = t.getStartHour();
		curr_MIN = t.getStartMinute();
		curr_SEC = t.getStartSecond();

		Serial.println(String("Current Time: ") +
			curr_HOUR + ":" +
			curr_MIN + ":" +
			curr_SEC);
	}
	else if (t.isStartSunrise())
	{
		Serial.println("Start at sunrise");
	}
	else if (t.isStartSunset())
	{
		Serial.println("Start at sunset");
	}
	else
	{
		// Do nothing
	}

	// Process timezone
	// Timezone is already added to start/stop time

	Serial.println(String("Time zone: ") + t.getTZ());

	// Get timezone offset (in seconds)
	Serial.println(String("Time zone offset: ") + t.getTZ_Offset());

	// Process weekdays (1. Mon, 2. Tue, 3. Wed, ...)

	//for (int i = 1; i <= 7; i++) {
	//	if (t.isWeekdaySelected(i)) {
	//		Serial.println(String("Day ") + i + " is selected");
	//	}
	//}

	Serial.println();
}


/* When the hardware is connected to Blynk cloud, it will call this method automatically.
	*- This method will update the start/stop time, timezone and days on the time input widget.
	*- This method can be used to set the default time on the time input widget when the hardware starts but it requires to
		set the same time as alarm on the RTC, so it will turn the cleaner ON when the alarm goes off.
*/
/*BLYNK_CONNECTED() {
	//seconds from the start of a day. 0 - min, 86399 - max
	int startAt = 5 * 60; //00:05

	//seconds from the start of a day. 0 - min, 86399 - max
	int stopAt = (60 + 5) * 60; //01:05

	//timezone
	//full list of supported timezones could be found here
	//https://www.mkyong.com/java/java-display-list-of-timezone-with-gmt/
	char tz[] = "Asia/Karachi";

	Blynk.virtualWrite(V1, startAt, stopAt, tz);

	//you may also pass day
	//char days[] = "1"; //Monday
	//Blynk.virtualWrite(V1, startAt, stopAt, tz, days);

	//or days
	//char days[] = "1,2,3"; //Monday, Tuesday, Wednesday
	//Blynk.virtualWrite(V1, startAt, stopAt, tz, days);
}

*/


void setup()
{
	Serial.begin(9600);
	delay(10);

	// Set ESP8266 baud rate
	EspSerial.begin(ESP8266_BAUD);
	delay(10);

	//Set Serial2 for GSM and configure GSM in Text_mode
	GSMInit();

	//Initiallize LCD 
	lcd.begin(16, 2);		//col = 16, row = 2(starts from 0) 
	lcd.print("** Smart Solar *");
	lcd.setCursor(0, 1);	//sets cursor to col=0, row=1 
	lcd.print("  Panel Cleaner ");
	for (int p = 0; p <= 5; p++) {
		BlinkLCDText();
	}

	lcd.clear();		//clear Text on LCD and sets cursor to col=0, row=0
	lcd.blink();		//shows blinking cursor 

	//connects to Wifi and Blynk Server
	Blynk.begin(auth, wifi, ssid, pass);

	//Initiallize RTC and sets 00:00:00 time on RTC
	RTC::RTCInit();

	//Initiallize Sensor Pins
	Init();

	// Initiallize all motors of Cleaner.
	cleaner.Init();

}

void loop()
{
	Blynk.run();
	//send SMS to the user when there isn't enough water in the tank for the Wet Cleaning.
	if (readWaterLevel() == 0) {
		lcd.setCursor(0, 0);
		lcd.print("Water Tank empty");	//It is Raining
		SendSms(PHONE_NO, WARN_WATER_LEVEL);
	}

	//send SMS to the user when it's raining.
	if (readRainSensor() == 0) {
		lcd.setCursor(0, 0);
		lcd.print("It's Raining !");
		SendSms(PHONE_NO, WARN_RAINING);
	}

	if (RTC::isAlarmTime()) {
		if (isFreeA1) {
			Serial.println("Alarm A1 is about to set");
			RTC::setAlarmA1();
			isFreeA1 = false;
		}
		/***********************  Currently Stop Time for Cleaner is not implemented********************************/
		/***********************  Would be better to implement it with 4 ultrasonic Sensors*************************/
		/*
		else if (isFreeA2) {
			Serial.println("Alarm A2 is about to set");
			RTC::setAlarmA2();
			isFreeA2 = false;
		}*/
	}

	if (RTC::isTrigA1()) {
		Serial.println("\nCleaner turned ON after A1 Alarm Trigger");
		isFreeA1 = true;
		RTC::resetFlagsA1();
		SendSms(PHONE_NO, WARN_CLEANER_RUN_ALARM_TRIG);
		DryCleaning();
		WetCleaning();
	}
	/*****************************no Stop Time for the Cleaner is implemented*******************************/
	/*
	if (RTC::isTrigA2()) {
		Serial.println("\nCleaner turned OFF after A2 Alarm Trigger");
		isFreeA2 = true;
		RTC::resetFlagsA2();
	}*/
}


void updateSerial()
{
	delay(500);
	while (Serial.available())
	{
		Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
	}
	while (Serial2.available())
	{
		Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
	}
}

void GSMInit() {
	//Begin serial communication with Arduino and SIM900
	Serial2.begin(9600);

	Serial.println("Initializing...");
	delay(1000);

	Serial2.println("AT"); //Handshaking with SIM900
	updateSerial();

	Serial2.println("AT+CMGF=1"); // Configuring TEXT mode, The text message entered followed by a �Ctrl+z� character is							// treated as SMS.
	updateSerial();

}
/************************* SEND SMS to the Phone Number specified******************************
	*- ph_no format: "\"+ZZxxxxxxxxxx\"" where ZZ= country code, xxxxxxxxxx = phone number

	*- AT command: AT+CMGS=+ZZxxxxxxxxxx command for specifying phone number for sms
*/
void SendSms(const char* ph_no, const char* text_msg) {
	char at_ph_cmd[30]; //AT command buffer for specifying the phone number
	// AT+CMGS command used with ph_no given to this function
	snprintf(at_ph_cmd, sizeof(at_ph_cmd), "AT+CMGS=%s", ph_no);
	//write AT command buffer to GSM over Serial.
	//uncomment this line if ph_no is used.
	Serial2.println(at_ph_cmd);

	//phone number and AT command is specified in single line
	//uncomment this line if phone_no is specified here.
	//Serial2.println("AT+CMGS=\"+ZZxxxxxxxxxx\"");	//change ZZ with country code and xxxxxxxxxxx with phone number to sms
	updateSerial();

	//uncomment this line if you want to send this sms text
	//Serial2.print("Evoo-Bot V1.0 by Helios Energy co. Jordan"); //text content

	//uncomment this line if you want to send text_msg given to this function
	Serial2.print(text_msg);
	updateSerial();

	Serial2.write(26); //CTRL+Z is 26 decimal in ASCII
}

/*********************************************************************************************************************
Moves Cleaner Forward and Backward using Segment to Segment movement from right to left.
**********************************************************************************************************************/
void DryCleaning(void) {
	lcd.setCursor(0, 0);
	lcd.print("Cleaning Solar !");
	lcd.setCursor(0, 1);
	lcd.print("Dry Cleaning !!");
	delay(500);
	while (1) {
		lcd.clear();
		lcd.print("Cleaner Forward !");
		int f_Distance = GetForDistance();
		while (!(f_Distance > 0 && f_Distance <= 4)) {
			cleaner.forward(FOR_SPEED_MT);
			delay(20);
			f_Distance = GetForDistance();
			if (f_Distance > 0 && f_Distance <= 4) {		// cleaner reached extreme forward edge.
				cleaner.for_stop(); //forward/backward motor stops
				delay(20);
				current_segment++;
				lcd.setCursor(0, 1);
				lcd.print("Edge Detected !!");
				Serial.print("Current Segment = ");
				Serial.println(current_segment);
				//run cleaner horizontally towards left side with SEGMENT_DELAY (change SEGMENT_DELAY by trial and error)
				cleaner.segment_left(HOR_SPEED_MT);
				//check if the left edge of Solar Panel is detected
				int h_Distance = GetGuardDistance();
				if (h_Distance >= 12 && h_Distance <= 27) {
					Serial.println("Solar Panel Left Edge Detected !!");
					total_segments = current_segment;
					Serial.print("Total Segments = ");
					Serial.println(total_segments);
					cleaner_allow_back = false;
				}
				break;
			}
		}
		lcd.clear();
		lcd.print("Cleaner backward");
		int b_Distance = GetBackDistance();
		while (!(b_Distance >= 6 && b_Distance <= 10) && cleaner_allow_back) {
			cleaner.backward(FOR_SPEED_MT);
			delay(20);
			b_Distance = GetBackDistance();
			if (b_Distance >= 6 && b_Distance <= 10) {
				cleaner.for_stop();
				delay(20);
				lcd.setCursor(0, 1);
				lcd.print("Edge Detected !!");
				break;
			}
		}
		if (total_segments == current_segment) {
			Serial.println("Dry Cleaning Done !!!");
			lcd.clear();
			lcd.print("Dry Cleaning");
			lcd.setCursor(0, 1);
			lcd.print("	  Done !!   ");
			//Turn on Water Pump 
			w_pump_start();
			cleaner_allow_back = true;
			break;
		}
	}
}

/*********************************************************************************************************************
moves cleaner from left to right using Segment to Segment movement(Wet Cleaning).
**********************************************************************************************************************/
void WetCleaning(void) {
	lcd.clear();
	lcd.print("Wet Cleaning");
	lcd.setCursor(0, 1);
	lcd.print("   Started !!");
	delay(100);
	while (current_segment != 0) {
		cleaner.segment_right(HOR_SPEED_MT);
		current_segment--;
		//this is the point where solar panel edge is detected, next segment is cleaner guard
		if (current_segment == current_segment - 1) {
			Serial.println("Right Solar Edge Detected!!");
			cleaner.hor_stop();
			delay(20);
			//Turn OFF Water Pump
			w_pump_stop();

		}
		if (current_segment == 0) {
			cleaner.hor_stop();
			delay(20);
			break;
		}
	}

	if (current_segment == 0) {
		int b_Distance = GetBackDistance();
		while (!(b_Distance >= 6 && b_Distance <= 10)) {
			cleaner.backward(FOR_SPEED_MT);
			delay(20);
			b_Distance = GetBackDistance();
			if (b_Distance >= 6 && b_Distance <= 10) {
				lcd.clear();
				lcd.print("Wet Cleaning");
				lcd.setCursor(0, 1);
				lcd.print("    Done !!   ");
				//Cleaner is now under Cleaner Guard.
				cleaner.stop();		//stop the whole cleaner
				delay(20);
				break;
			}
		}
	}
}


void BlinkLCDText() {
	lcd.noDisplay();
	delay(500);
	lcd.display();
	delay(500);
}
