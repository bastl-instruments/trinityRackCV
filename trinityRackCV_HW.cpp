
/*** PERMANENT SETTINGS***/

#include "trinityRackCV_HW_settings.h"


/*** ACTUAL CODE ***/

#include <Arduino.h>
#include "trinityRackCV_HW.h"
#include <shiftRegisterFast.h>
#include <avr/pgmspace.h>
#include <fastAnalogRead.h>
//#include <spiShared.h>
#include <SPI.h>

//#define PIN B,5
#define CV_MOVED_TOLERANCE 1
//using namespace fastAnalogRead;

//#include <SdFat.h>

// Declaration of instance (for use in interrupt service routine)
trinityRackCV_HW hardware;


#define UINT16_MAX 65535
#define MAX_ADDR 131067

#define DACA 3
#define DACB 15
#define DACC 7

#define DACD 1
#define DACE 5
#define DACF 13

#define DACG 9
#define DACH 11

unsigned char dacNumber[8]={ DACA,DACB,DACC, DACD,DACE,DACF, DACG,DACH};


//prog_uchar colorBit[NUMBER_OF_COLORS] PROGMEM = {


unsigned char analogPin[6]={
  ANALOG_PIN_1, ANALOG_PIN_2, ANALOG_PIN_3,  ANALOG_PIN_4, ANALOG_PIN_5, ANALOG_PIN_6};

void trinityRackCV_HW::init(void(*clockInCallback)()) {

	cli();


	bit_dir_inp(INPUT);




	bit_dir_outp(LOAD_PIN);
	//bit_dir_outp(PIN); //debug
	DACInit();

	bit_set(INPUT);




	// store callback pointer for changed buttons
	 this->clockInCallback = clockInCallback;

	// Disable Timer1 interrupt
	//TIMSK1 &= ~_BV(TOIE1);

	// TIMER 2
	TCCR2A = (1 << WGM21);  // turn on CTC mode
	TIMSK2 |= (1 << OCIE2A);// enable interrupt
	TCCR2B = B00000111;	    //prescaler = 1024
	OCR2A = (F_CPU/1024)/(updateFreq);
	TCNT2  = 0;

	fastAnalogRead::init(); // for analog read
	CVCount=0;
	fastAnalogRead::connectChannel(CVCount);
	fastAnalogRead::startConversion();

	CVMovedHash=0;


	sei();

}







/**** TRIGGER ****/
void trinityRackCV_HW::setDAC(uint8_t number, uint8_t value){
	dacValues[number]=value;
}




uint8_t trinityRackCV_HW::getCVValue(uint8_t index){
	return CVValues[index];
}

void trinityRackCV_HW::isr_updateADC(){

	if(fastAnalogRead::isConversionFinished()){
	lastCVValues[CVCount]=CVValues[CVCount];
	CVValues[CVCount]=255-(fastAnalogRead::getConversionResult()>>2);
	int difference=(int)lastCVValues[CVCount]-CVValues[CVCount];
	difference=abs(difference);
	if(difference>CV_MOVED_TOLERANCE) bitWrite(CVMovedHash,CVCount,1);
	else bitWrite(CVMovedHash,CVCount,0);
	CVCount++;
	if(CVCount>=6) CVCount=0;
	fastAnalogRead::connectChannel(analogPin[CVCount]);
	fastAnalogRead::startConversion();
	}

}


bool trinityRackCV_HW::CVMoved(uint8_t index){
	return bitRead(CVMovedHash,index);
}

void trinityRackCV_HW::isr_updateDAC(){
	if(dacCount<7) dacCount++;
	else dacCount=0;
	DACWrite(dacCount,dacValues[dacCount]);

}

void trinityRackCV_HW::DACInit(){
 // pinMode (LOAD_PIN, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);  // Most Significant bit first.
  SPI.setClockDivider(SPI_CLOCK_DIV16);  //16MHz divided by 16 = 1MHz
  SPI.setDataMode(SPI_MODE1);  // zero based clock, data on falling edge, seems like the correct setting
  bit_clear(LOAD_PIN);
  zeroDACs();
}

void trinityRackCV_HW::zeroDACs(){
  for (int i=0;i< 8; i++){
    DACWrite(i,0);
  }
}




void trinityRackCV_HW::DACWrite(unsigned char _channel, unsigned char _level) {

  bit_set(LOAD_PIN);
  SPI.transfer(dacNumber[_channel]);
  SPI.transfer(_level);
  bit_clear(LOAD_PIN);

}


void trinityRackCV_HW::isr_updateClockIn(){
	if(clockInCallback!=0){
		bool newState=!bit_read_in(INPUT);
		if(newState && !clockInState) clockInCallback();
		clockInState=newState;
	}
}

bool trinityRackCV_HW::getClockState(){
	return clockInState;
}

/**** TIMING ****/

uint32_t trinityRackCV_HW::getElapsedBastlCycles() {
	return bastlCycles;
}

uint16_t trinityRackCV_HW::getBastlCyclesPerSecond() {
	return (F_CPU/1024)/OCR2A;
}


/**** INTERRUPT ****/

ISR(TIMER2_COMPA_vect) { //56us :)


//	bit_set(PIN);
	hardware.incrementBastlCycles();
	hardware.isr_updateClockIn();
	hardware.isr_updateADC();
	hardware.isr_updateDAC();

	//hardware.isr_updateNextLEDRow();   // ~84us

//	bit_clear(PIN);


}


