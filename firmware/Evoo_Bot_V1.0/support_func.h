#pragma once


#ifndef SUPPORT_FUNC_H
#define SUPPORT_FUNC_H

long GetForDistance(void);
long GetBackDistance(void);
long GetGuardDistance(void);
void Init(void);
void EnableWlSen(void);
void DisableWlSen(void);
void w_pump_start(void);
void w_pump_stop(void);
uint8_t readWaterLevel(void);
uint8_t readRainSensor(void);

#endif