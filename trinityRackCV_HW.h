
#include "trinityRackCV_HW_settings.h"

#ifndef TRINITYRACKCV_HW_H_
#define TRINITYRACKCV_HW_H_

#include <IHWLayer.h>
#include <Arduino.h>
#include <avr/pgmspace.h>


class trinityRackCV_HW  { //: public IHWLayer


public:
	enum TriggerState{ON,OFF};


	// sets up all the pins, timers and SPI interface
	// call this before using any other method from this class
	void init(void(*clockInCallback)());

	/***KNOBS***/

	//
	virtual uint8_t getCVValue(uint8_t index);
	uint8_t getLastCVValue(uint8_t index){return lastCVValues[index];}
	bool CVMoved(uint8_t index);

	void setDAC(uint8_t number, uint8_t value);
	bool getClockState();

	/**TIMING**/

	// the number of bastl cycles elapsed since startup
	// this number will overflow after some minutes; you have to deal with that in the layer above
	// using a longer datatype would prevent this but at the cost of longer computation time
	uint32_t getElapsedBastlCycles();

	// returns the relation between bastl cycles and seconds
	// this value is dependent on the hardware update frequency that you can set by a define
	// use this to map real time processes (like BMP) to bastlCycles
	uint16_t  getBastlCyclesPerSecond();




	// only called by ISR routine.
	// they would be declared private but this would prevent the ISR from accessing them
	// there are workarounds for this but as they come at a cost I just left it like this


	void isr_updateADC();
	void isr_updateDAC();
	void isr_updateClockIn();

	inline void incrementBastlCycles() {bastlCycles++;}

	/**EEPROM**/

	virtual void readEEPROM(uint8_t pageNumb, uint8_t* byteArray, uint16_t numbBytes){}
	virtual void readEEPROMDirect(uint16_t address, uint8_t* byteArray, uint16_t numbBytes){}
	virtual bool isEEPROMBusy(){return true;}


private:
	/**TIMING**/
	uint32_t bastlCycles;
	//bool inBetween(uint8_t value, uint8_t border1, uint8_t border2);
	bool clockInState;
	/**BUTTONS**/

	uint8_t CVMovedHash;
	uint8_t dacCount;

	uint8_t dacValues[8];
	uint8_t CVValues[6];
	uint8_t lastCVValues[6];
	uint8_t CVCount;

	void zeroDACs();
	void DACInit();
	void DACWrite(unsigned char _channel, unsigned char _level);

	void (*clockInCallback)();

};




#endif
