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


/*
#define CLOCK_PIN 5
#define LATCH_PIN 4
#define DATA_PIN 3
*/
#define ANALOG_PIN_1 5
#define ANALOG_PIN_2 1
#define ANALOG_PIN_3 3

#define ANALOG_PIN_4 4
#define ANALOG_PIN_5 0
#define ANALOG_PIN_6 2
/*
#define ANALOG_PIN_1 C,5
#define ANALOG_PIN_2 C,4
#define ANALOG_PIN_3 C,3

#define ANALOG_PIN_4 C,2
#define ANALOG_PIN_5 C,1
#define ANALOG_PIN_6 C,0
*/

#define INPUT D,3






/*
#define BUTTON_PIN 	17
#define BUTTON_2_PIN 4
#define BUTTON_3_PIN 2
#define BUTTON_4_PIN 7


#define LED_PIN 18 //2
#define LED_2_PIN 5
#define LED_3_PIN 3
#define LED_4_PIN 6
*/



// the frequency at which the leds are updated.
// when set too high, resources are wasted, when set too low flikering occurs
// the value is in hertz and can be set from 16 to 255

static const uint16_t updateFreq = 1000;  // because its cool dude ! - please update next year



// properties of the led blinking
// total defines the frequency of one blink cycle
// duty defines when the led state is switched
// the values are relative to the frequency, so when you want dimming instead of blinking, you will need to increase that value
//  -------
//        |
//        |
//        -------------
//  0    duty         total

#define blinkTotal 2
#define blinkDuty  1



//
#define BUFFER_DEPTH 40


// debug Pin
#define PIN D,3

#endif /* LITTLENERDHW_SETTINGS_H_ */
