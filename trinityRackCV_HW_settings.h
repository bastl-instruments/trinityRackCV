/*
 * TRINITYRACKCV_HW-settings.h
 *
 *  Created on: 15.07.2014
 *      Author: user
 */
#include <Arduino.h>

#ifndef LITTLENERDHW_SETTINGS_H_
#define LITTLENERDHW_SETTINGS_H_


#define LOAD_PIN B,2


const uint8_t ANALOG_PIN_1 = 5;
const uint8_t ANALOG_PIN_2 = 1;
const uint8_t ANALOG_PIN_3 = 3;
const uint8_t ANALOG_PIN_4 = 4;
const uint8_t ANALOG_PIN_5 = 0;
const uint8_t ANALOG_PIN_6 = 2;


#define CLOCK_IN_PIN D,3

// debug Pin
#define PIN D,3




// hardware update frequency
static const uint16_t updateFreq = 2000;





#endif /* LITTLENERDHW_SETTINGS_H_ */
