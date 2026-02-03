/*
	Name:       SolarCleaner.ino
	Created:	4/24/2021 8:23:33 PM
	Author:     HP-PC\hp
*/

#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include "support_rtc.h"

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600	//Don't forget to permanently change the default baud rate of ESP fromm 115200 to 9600 using AT							  //command

ESP8266 wifi(&EspSerial);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";

// Your WiFi credentials. Set password to "" for open networks.
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

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

/* This method is called automatically when the user selects the start/stop time on the time input widget
	*- Remember- even though you change nothing(start/stop, timezone, days) and press OK button in the time input widget, this method will be called with the current start/stop, timezone and days.
*/

BLYNK_WRITE(V1) {
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
     we would've wrong times in support_rtc.cpp. therefore, two variables (on_time_str and off_time_str were created globally)
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

	// Process stop time

	if (t.hasStopTime())
	{
		//t.getStartHour = Alarm Time in Hours
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
		Serial.println("Stop at sunrise");
	}
	else if (t.isStopSunset())
	{
		Serial.println("Stop at sunset");
	}
	else
	{
		// Do nothing: no stop time was set
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
		set the same time on the RTC, so it will turn the cleaner ON when the time goes off.
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
	RTC::RTCInit();
	
	Blynk.begin(auth, wifi, ssid, pass);


}

void loop()
{
	Blynk.run();
  if(RTC::isAlarmTime()){
    if (isFreeA1) {
      Serial.println("Alrm A1 is about to set");
      RTC::setAlarmA1();
      isFreeA1 = false;
    }
    else if (isFreeA2) {
      Serial.println("Alrm A2 is about to set");
      RTC::setAlarmA2();
      isFreeA2 = false;
    }  
  }
	

	if (RTC::isTrigA1()) {
		Serial.println("\nCleaner turned ON after A1 Alarm Trigger");
		isFreeA1 = true;
		RTC::resetFlagsA1();
	}

	if (RTC::isTrigA2()) {
		Serial.println("\nCleaner turned OFF after A2 Alarm Trigger");
		isFreeA2 = true;
		RTC::resetFlagsA2();
	}

}
