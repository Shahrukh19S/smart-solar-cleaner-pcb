#include "NewPing.h"
#include "support_func.h"
#include "Arduino.h"
#include "Globals.h"


NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(FOR_TRIG, FOR_ECH0, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(BACK_TRIG, BACK_ECH0, MAX_DISTANCE),
  NewPing(GUARD_TRIG, GUARD_ECH0, MAX_DISTANCE)
};


long GetForDistance(void) {
	delay(50);
	long  distance = sonar[0].ping_cm();
	return (distance);         //return distance in cm
}

long GetBackDistance(void) {
	delay(50);
	long  distance = sonar[1].ping_cm();
	return (distance);         //return distance in cm
}

long GetGuardDistance(void) {
	delay(50);
	long  distance = sonar[2].ping_cm();
	return (distance);         //return distance in cm
}

/*********************************************************************************************************************
Turn On the VCC Pin(HIGH) of Water level sensor to enable it.
**********************************************************************************************************************/
void EnableWlSen(void) {
	// Set to HIGH so power flows through the sensor(sensor enabled)
	digitalWrite(WL_SEN_EN, HIGH);
}


/*********************************************************************************************************************
Turn Off the VCC Pin(LOW) of Water level sensor to disable it.
**********************************************************************************************************************/
void DisableWlSen(void) {
	// Set to LOW so no power flows through the sensor(sensor disabled)
	digitalWrite(WL_SEN_EN, LOW);
}

/*********************************************************************************************************************
Turn ON the Water Pump for Wet Cleaning (active Low Relay)
**********************************************************************************************************************/
void w_pump_start(void) {
	digitalWrite(W_PUMP, LOW);
}

/*********************************************************************************************************************
Turn OFF the Water Pump for Wet Cleaning (active Low Relay)
**********************************************************************************************************************/
void w_pump_stop(void) {
	digitalWrite(W_PUMP, HIGH);
}

/***********************************************************************************************************************
This function is for Water Level Sensor that has 3 Pins.
************************************************************************************************************************/
/*
uint8_t readWaterLevel(void) {
	EnableWlSen();						// turn on the sensor
	delay(10);							// wait 10 milliseconds
	int val = analogRead(WL_SEN_S);		// Read the analog value form sensor
	Serial.print("Water level: ");
	Serial.println(val);
	DisableWlSen();						// turn off the sensor
	return val;
}*/

/***********************************************************************************************************************
This function is for Water Level Sensor that has only 2 Pins.(WL_SEN_EN pin is not used)
************************************************************************************************************************/
uint8_t readWaterLevel(void) {
	int val = digitalRead(WL_SEN_S);		// Read the analog value form sensor
	Serial.print("Water level: ");
	Serial.println(val);
	return val;
}

uint8_t readRainSensor(void) {

#ifdef EN_DIGITAL_R_SEN

	int val = digitalRead(R_SEN);

#endif // EN_DIGITAL_R_SEN

#ifdef EN_ANALOG_R_SEN
	int val = analogRead(R_SEN);

#endif // EN_ANALOG_R_SEN

	Serial.print("Rain Sensor : ");
	Serial.println(val);
	return val;
}

void Init(void) {
	/********************* uncomment this line if 3 Pin Water level sensor is used******************/
	//pinMode(WL_SEN_EN, OUTPUT);
	pinMode(W_PUMP, OUTPUT);
	pinMode(WL_SEN_S, INPUT);
	pinMode(R_SEN, INPUT);
	//DisableWlSen();	//uncomment this line if 3 Pin Water level sensor is used.
	digitalWrite(W_PUMP, HIGH);	//turn off W_PUMP initially(active Low Relay OUTPUT)

}