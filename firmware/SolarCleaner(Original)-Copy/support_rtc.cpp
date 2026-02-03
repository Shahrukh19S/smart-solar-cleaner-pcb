#include "support_rtc.h"
#include <Wire.h>
#include "string.h"
#include "ds3231.h"

#define BUFF_MAX 128

uint8_t wake_HOUR = 00;
uint8_t wake_MINUTE = 00;
uint8_t wake_SECOND = 00;

uint8_t Tame[8];	//renamed to Tame, because Time is defined in arduino default libraries
uint8_t Alarm_ID = 0;

struct TIME {
	char* LampTimesAry[MAX_ALARM_TIME];

}Alarm;

/* saves the Alarms time here, so it can be used later for setting the Alarm
	*-array_id: 0 = On Time , 1 = Off Time
  *- index: 1= On Time, 2 = Off Time, 0 = undefined( alarm not set)
	*- value: Alarm time as string in format %02d:%02d:%02d i.e. 12:04:50
*/
void RTC::saveAlarmTime(const uint8_t index, const char* value) {
	Alarm_ID = index;
	Alarm.LampTimesAry[index - 1] = (char*)value;

}

/*****************************************************************************
Function description : gets the Alarm time in %02d:%02d:%02d format as String
*****************************************************************************/
static char* getAlarmTime(const uint8_t index) {
	return Alarm.LampTimesAry[index];
}

/*****************************************************************************
Function description : tells wheather its time to set new alarm on RTC
*****************************************************************************/
unsigned char RTC::isAlarmTime(void) {
	if (Alarm_ID != 0)
		return TRUE;

	return FALSE;
}


/*****************************************************************************
Function description : tells wheather A1 alarm is triggerred
*****************************************************************************/
uint8_t RTC::isTrigA1(void) {
	return DS3231_triggered_a1();
}

/*****************************************************************************
Function description : tells wheather A2 alarm is triggerred
*****************************************************************************/
uint8_t RTC::isTrigA2(void) {
	return DS3231_triggered_a2();
}

/*****************************************************************************
Function description : resets A1 flag of RTC
*****************************************************************************/
void RTC::resetFlagsA1(void) {
	Alarm_ID = 0;
	DS3231_clear_a1f();
}

/*****************************************************************************
Function description : reset A2 flag of RTC
*****************************************************************************/
void RTC::resetFlagsA2(void) {
	Alarm_ID = 0;
	DS3231_clear_a2f();
}

/*****************************************************************************
Function description : set A1 Alarm on RTC where time is saved in WAKE_HOUR, WAKE_MINUTE, WAKE_SECOND
*****************************************************************************/
static void set_alarm_A1(void)
{

	// flags define what calendar component to be checked against the current time in order
	// to trigger the alarm - see datasheet
	// A1M1 (seconds) (0 to enable, 1 to disable)
	// A1M2 (minutes) (0 to enable, 1 to disable)
	// A1M3 (hour)    (0 to enable, 1 to disable) 
	// A1M4 (day)     (0 to enable, 1 to disable)
	// DY/DT          (dayofweek == 1/dayofmonth == 0)
	uint8_t flags[5] = { 0, 0, 0, 1, 1 };

	// set Alarm1
	DS3231_set_a1(wake_SECOND, wake_MINUTE, wake_HOUR, 0, flags);

	// activate Alarm1
	DS3231_set_creg(DS3231_INTCN | DS3231_A1IE);
	//	isAlrmSet1 = true;
}

/*****************************************************************************
Function description : set A2 Alarm on RTC where time is saved in WAKE_HOUR, WAKE_MINUTE, WAKE_SECOND
*****************************************************************************/
static void set_alarm_A2(void)
{
	// flags define what calendar component to be checked against the current time in order
	// to trigger the alarm - see datasheet
	// A1M1 (seconds) (0 to enable, 1 to disable)
	// A1M2 (minutes) (0 to enable, 1 to disable)
	// A1M3 (hour)    (0 to enable, 1 to disable) 
	// A1M4 (day)     (0 to enable, 1 to disable)
	// DY/DT          (dayofweek == 1/dayofmonth == 0)
	uint8_t flags[4] = { 0, 0, 1, 1 };

	// set Alarm1
	DS3231_set_a2(wake_MINUTE, wake_HOUR, 0, flags);

	// activate Alarm1
	DS3231_set_creg(DS3231_INTCN | DS3231_A2IE);
	//isAlrmSet2 = true;
}

/*****************************************************************************
Function description : calculates the ON time( elaps time i.e. 00:05 (5 minutes)), adds it to current Time of RTC and 
						save it in WAKE_HOUR, WAKE_MINUTE, WAKE_SECOND
*****************************************************************************/
void RTC::setAlarmA1(void) {
	char log[60];
	int ServerHour, ServerMin, ServerSec;
	//getting the previous saved Alarm ON time
	sscanf(getAlarmTime(0), "%02d:%02d:%02d", &ServerHour, &ServerMin, &ServerSec);

	snprintf(log, sizeof(log), "Server alarm Time is %02d:%02d:%02d", ServerHour, ServerMin, ServerSec);
	Serial.println(log);

	//getting the current Time from the RTC
	struct ts t;
	DS3231_get(&t);

	wake_HOUR = t.hour + ServerHour;
	wake_MINUTE = t.min + ServerMin;
	wake_SECOND = ServerSec;
	if (wake_HOUR > 23) {
		wake_HOUR = wake_HOUR - 24;
	}
	if (wake_MINUTE > 60) {
		wake_MINUTE = wake_MINUTE - 60;
		wake_HOUR = wake_HOUR + 1;
	}

	snprintf(log, sizeof(log), "The Alarm Time for A1 is = %02d:%02d:%02d", wake_HOUR, wake_MINUTE, wake_SECOND);
	Serial.println(log);

	set_alarm_A1();

	//setting it to 0 after the alarm is set on RTC
	wake_HOUR = 0;
	wake_MINUTE = 0;
	wake_SECOND = 0;

}

/*****************************************************************************
Function description : calculates the OFF time( elaps time i.e. 00:05 (5 minutes)), adds it to current Time of RTC and
						save it in WAKE_HOUR, WAKE_MINUTE, WAKE_SECOND
*****************************************************************************/
void RTC::setAlarmA2(void) {
	char log[60];
	int ServerHour, ServerMin, ServerSec;
	//getting the previous saved Alarm OFF time
	sscanf(getAlarmTime(1), "%02d:%02d:%02d", &ServerHour, &ServerMin, &ServerSec);
	snprintf(log, sizeof(log), "Server alarm Time is %02d:%02d:%02d", ServerHour, ServerMin, ServerSec);
	Serial.println(log);

	//getting the current Time from the RTC
	struct ts t;
	DS3231_get(&t);

	wake_HOUR = t.hour + ServerHour;
	wake_MINUTE = t.min + ServerMin;
	wake_SECOND = ServerSec;
	if (wake_HOUR > 23) {
		wake_HOUR = wake_HOUR - 24;
	}
	if (wake_MINUTE > 60) {
		wake_MINUTE = wake_MINUTE - 60;
		wake_HOUR = wake_HOUR + 1;
	}

	snprintf(log, sizeof(log), "The Alarm Time for A2 is = %02d:%02d:%02d", wake_HOUR, wake_MINUTE, wake_SECOND);
	Serial.println(log);

	set_alarm_A2();

	//setting it to 0 after the alarm is set on RTC
	wake_HOUR = 0;
	wake_MINUTE = 0;
	wake_SECOND = 0;

}

/*****************************************************************************
Function description : sets the initial time on RTC when the Device first boots up
Input parameter : cmd:  TssmmhhWDDMMYYYY aka set time ss->second,mm->minute,hh->hour,W->week 0-6.
				: cmdsize:length of cmd->16 mostly
*****************************************************************************/
static void parse_cmd(char *cmd, int cmdsize)
{
	uint8_t i;
	uint8_t reg_val;
	char buff[BUFF_MAX];
	struct ts t;

	//snprintf(buff, BUFF_MAX, "cmd was '%s' %d\n", cmd, cmdsize);
	//Serial.print(buff);

	// TssmmhhWDDMMYYYY aka set time
	if (cmd[0] == 84 && cmdsize == 16) {
		//T355720619112011
		t.sec = inp2toi(cmd, 1);
		t.min = inp2toi(cmd, 3);
		t.hour = inp2toi(cmd, 5);
		t.wday = cmd[7] - 48;
		t.mday = inp2toi(cmd, 8);
		t.mon = inp2toi(cmd, 10);
		t.year = inp2toi(cmd, 12) * 100 + inp2toi(cmd, 14);
		DS3231_set(t);
		Serial.println("OK");
	}
	else if (cmd[0] == 49 && cmdsize == 1) {  // "1" get alarm 1
		DS3231_get_a1(&buff[0], 59);
		Serial.println(buff);
	}
	else if (cmd[0] == 50 && cmdsize == 1) {  // "2" get alarm 1
		DS3231_get_a2(&buff[0], 59);
		Serial.println(buff);
	}
	else if (cmd[0] == 51 && cmdsize == 1) {  // "3" get aging register
		Serial.print("aging reg is ");
		Serial.println(DS3231_get_aging());
	}
	else if (cmd[0] == 65 && cmdsize == 9) {  // "A" set alarm 1
		DS3231_set_creg(DS3231_INTCN | DS3231_A1IE);
		//ASSMMHHDD
		for (i = 0; i < 4; i++) {
			Tame[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // ss, mm, hh, dd
		}
		uint8_t flags[5] = { 0, 0, 0, 0, 0 };
		DS3231_set_a1(Tame[0], Tame[1], Tame[2], Tame[3], flags);
		DS3231_get_a1(&buff[0], 59);
		Serial.println(buff);
	}
	else if (cmd[0] == 66 && cmdsize == 7) {  // "B" Set Alarm 2
		DS3231_set_creg(DS3231_INTCN | DS3231_A2IE);
		//BMMHHDD
		for (i = 0; i < 4; i++) {
			Tame[i] = (cmd[2 * i + 1] - 48) * 10 + cmd[2 * i + 2] - 48; // mm, hh, dd
		}
		uint8_t flags[5] = { 0, 0, 0, 0 };
		DS3231_set_a2(Tame[0], Tame[1], Tame[2], flags);
		DS3231_get_a2(&buff[0], 59);
		Serial.println(buff);
	}
	else if (cmd[0] == 67 && cmdsize == 1) {  // "C" - get temperature register
		Serial.print("temperature reg is ");
		Serial.println(DS3231_get_treg());
	}
	else if (cmd[0] == 68 && cmdsize == 1) {  // "D" - reset status register alarm flags
		reg_val = DS3231_get_sreg();
		reg_val &= B11111100;
		DS3231_set_sreg(reg_val);
	}
	else if (cmd[0] == 70 && cmdsize == 1) {  // "F" - custom fct
		reg_val = DS3231_get_addr(0x5);
		Serial.print("orig ");
		Serial.print(reg_val);
		Serial.print("month is ");
		Serial.println(bcdtodec(reg_val & 0x1F));
	}
	else if (cmd[0] == 71 && cmdsize == 1) {  // "G" - set aging status register
		DS3231_set_aging(0);
	}
	else if (cmd[0] == 83 && cmdsize == 1) {  // "S" - get status register
		Serial.print("status reg is ");
		Serial.println(DS3231_get_sreg());
	}
	else {
		Serial.print("unknown command prefix ");
		Serial.println(cmd[0]);
		Serial.println(cmd[0]);
	}
}

/*****************************************************************************
Function description : Initiallize and sets the initial time on RTC.
*****************************************************************************/
void RTC::RTCInit(void) {
	Wire.begin();
	DS3231_init(DS3231_INTCN);
	DS3231_clear_a1f();
	DS3231_clear_a2f();
	parse_cmd("T000000420072020", 16);
	struct ts t;
	DS3231_get(&t);
	char data[40];
	snprintf(data, sizeof(data), "\nDS3231 current time is %02d:%02d", t.hour, t.min);
	Serial.println(data);
}
