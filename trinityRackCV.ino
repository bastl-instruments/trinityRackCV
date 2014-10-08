
#ifdef EXTERNAL_IDE

#include <Arduino.h>
#include <EEPROM.h>

int main(void) {

  init();
  setup();

  while(true) {
    loop();
  }
}

#endif

#include <portManipulations.h>
#include "trinityRackCV_HW.h"
#include "ADSR.h"
ADSR envelope[6];

extern trinityRackCV_HW hardware;

/*
void stepperStep() {
	player->stepFourth();
	synchronizer.doStep();
}
*/


void noteOn(unsigned char note, unsigned char velocity, unsigned char channel) {

}

void noteOff(unsigned char note, unsigned char velocity, unsigned char channel) {

}

void midiNoteOnIn(unsigned char channel, unsigned char note, unsigned char velocity) {


}

void midiNoteOffIn() {

}

void test(uint8_t v) {
	//if(hardware.getButtonState(v) == IHWLayer::UP)

		//if(hardware.getButtonState(v)==IHWLayer::DOWN) hardware.setLED(v,IHWLayer::ON);
		//else hardware.setLED(v,IHWLayer::OFF);
}



uint8_t selectedChannel=255;

uint32_t clockInTime,lastClockInTime, channelTime, lastChannelTime;
uint16_t counter;
bool flop;
uint8_t currentPreset=0;
uint8_t parameter[6][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,20}};

#define NUMBER_OF_MODES 8

void buttonCall(uint8_t v) {

}



long myMap(long x,  long in_max,  long out_max)
{
  return (x) * (out_max) / (in_max);
}


uint8_t divider;
uint32_t offset[6];
// 2 ON OFF TIME STANDART
// 3 CHANNEL
// 3 TIME
uint8_t counterLeft;
uint8_t multiplication;

void renderMultiplier(){

	if (counterLeft != 0) {

			if ((hardware.getElapsedBastlCycles() - lastChannelTime) > (clockInTime / (multiplication))) {

				counter++;
				unsigned char subStepsFromLastStep = multiplication - counterLeft;

				//Next line is handling problems with rounding so instead of increasing the last triggered
				//To the current time we increase it by division of the step multiplied by number of current step
				//That should have little better precision
				lastChannelTime = channelTime + (( clockInTime * subStepsFromLastStep) / (multiplication));
				counterLeft--;

			}
		}

		//And here we trigger steps if there are some and the time from last step is lower then
		//Minimum distance in between steps that could be set by public setter
		if ((counter > 0) &&
			((hardware.getElapsedBastlCycles() - lastClockInTime) > 20)) {
			//-do step

			lastClockInTime = hardware.getElapsedBastlCycles();
			counter--;
		}


}
void doMultStep(){
		counter += counterLeft;
		counterLeft = ((multiplication) - 1);
		if (flop) {
		clockInTime = hardware.getElapsedBastlCycles() - channelTime;
		}
		channelTime = hardware.getElapsedBastlCycles();
		lastChannelTime = hardware.getElapsedBastlCycles();
		counter++;
		renderMultiplier();
		flop = true;
}








void clockCall() {


}


void setup() {
	Serial.begin(9600);
	hardware.init(&clockCall);
	for(int i=0;i<6;i++){
	envelope[i].setAttackRate(1000);
	envelope[i].setDecayRate(2000);
	envelope[i].setReleaseRate(3000);
	envelope[i].setSustainLevel(0.5);
	envelope[i].setTargetRatioA(0.5);
	envelope[i].setTargetRatioDR(0.01);
	}
}
uint32_t gateTime;
bool gate[6];
uint32_t _time;
void loop() {
	if(_time!=hardware.getElapsedBastlCycles()){
		for(int i=0;i<6;i++){
				float fout=(float)255*envelope[i].process();
				uint8_t out=(int)(fout);
				hardware.setDAC(i,out);
			}
	}
	for(int i=0;i<6;i++){
	if((hardware.getCVValue(i)>>7) != gate[i]){
		gate[i]=hardware.getCVValue(i)>>7;
		envelope[i].gate(gate[i]);
	}
	}
	_time=hardware.getElapsedBastlCycles();
	/*
	if(_time-gateTime>7000){
		gateTime=_time;
		gate=!gate;
		for(int i=0;i<6;i++) envelope[i].gate(gate);
		Serial.println(gate);
	}

*/


//	for(int i=0;i<6;i++) Serial.print("  "),Serial.print(i),Serial.print(": "), Serial.print(hardware.getCVValue(i));
//	Serial.println(" ");

	//

}




