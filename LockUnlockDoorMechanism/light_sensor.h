#ifndef LIGHT_SENSOR_H_
#define LIGHT_SENSOR_H_

#include <stdbool.h>
#include <stdint.h>


void LsInitialize();        // initialize light sensor

void LsTrigRead();          //read light value

bool LsData();              // Check if there are any new readings

// Read the most recent light reading
uint32_t LsRaw();

#endif /* LIGHT_SENSOR_H_ */
