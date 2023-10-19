/*
 * clockupdate.h
 *
 * ECE 266 Lab 3 starter code, spring 2023
 *
 * Created by Zhao Zhang
 */

#ifndef CLOCKUPDATE_H_
#define CLOCKUPDATE_H_

#include "launchpad.h"
#include "seg7.h"

// Declare global variable seg7
extern Seg7Display seg7;

// Update clock digits and colon every half second
void ClockUpdate(uint32_t time);

#endif /* CLOCKUPDATE_H_ */

