#ifndef SUPPORT_RTC_H
#define SUPPORT_RTC_H

#include <stdint.h>

#define MAX_ALARM_TIME 2

#define TRUE 1
#define FALSE 0

namespace RTC {
	
	void setAlarmA1(void);
	void setAlarmA2(void);
	//void parse_cmd(char *cmd, int cmdsize);
	void saveAlarmTime(const uint8_t index, const char* value);
	unsigned char isAlarmTime(void);
	void RTCInit(void);
	uint8_t isTrigA1(void);
	uint8_t isTrigA2(void);
	void resetFlagsA1(void);
	void resetFlagsA2(void);

}


#endif // !SUPPORT_RTC_H
