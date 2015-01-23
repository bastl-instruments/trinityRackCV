
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
#include <lfoExtended.h>
#include <simpleSerialDecoder.h>
#include "trinityRackCV_HW.h"
#include "ADSR.h"
ADSR envelope[6];
lfoExtended<2000> LFO[6];
simpleSerialDecoder com;

/* channel TYPES
 *lfo
 *envelope
 *cv follow
 *interpolate
 *trigger
 */

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


#define NUMBER_OF_CHANNELS 6
#define NUMBER_OF_VALUES 3
uint8_t channelMode[NUMBER_OF_CHANNELS];
uint8_t channelValue[NUMBER_OF_CHANNELS][NUMBER_OF_VALUES];




void clockCall(uint8_t number){

}
void stepCall(uint8_t number){

}
void gateCall(uint8_t number){

}
void triggerCall(uint8_t number){

}
void startCall(uint8_t number){

}
void stopCall(uint8_t number){

}
void restartCall(uint8_t number){

}
void channelTriggerCall(uint8_t channel,uint8_t number){

}
void channelGateCall(uint8_t channel,uint8_t number){
	if(number==0) hardware.setDAC(channel,number);
	else hardware.setDAC(channel,255);
}
void channelCVCall(uint8_t channel,uint8_t number){
	 hardware.setDAC(channel,number);
}
void channelInterpolateFromCall(uint8_t channel,uint8_t number){

}
void channelInterpolateToCall(uint8_t channel,uint8_t number){

}
void channeInterpolateCall(uint8_t channel,uint8_t number){

}
void channelModeCall(uint8_t channel,uint8_t number){
	channelMode[channel]=number;
	reflectModeChange(channel);
}

void channelValueCall(uint8_t channel, uint8_t value, uint8_t number){
	channelValue[channel][value]=number;
	reflectValueChange(channel,value);
	/*
	 if(number>128) com.sendChannelGate(channel,1);
	 else com.sendChannelGate(channel,0);
	 hardware.setDAC(channel,number);
	 */
}


void setup() {
	//create callbacks
	com.init(9600);
	com.attachClockCallback(&clockCall);
	com.attachStepCallback(&stepCall);
	com.attachGateCallback(&gateCall);
	com.attachTriggerCallback(&triggerCall);
	com.attachStartCallback(&startCall);
	com.attachStopCallback(&stopCall);
	com.attachRestartCallback(&restartCall);
	com.attachChannelTriggerCallback(&channelTriggerCall);
	com.attachChannelGateCallback(&channelGateCall);
	com.attachChannelCVCallback(&channelCVCall);
	com.attachChannelInterpolateFromCallback(&channelInterpolateFromCall);
	com.attachChannelInterpolateToCallback(&channelInterpolateToCall);
	com.attachChannelInterpolateCallback(&channeInterpolateCall);
	com.attachChannelModeCallback(&channelModeCall);
	com.attachChannelValueCallback(&channelValueCall);


	//Serial.begin(9600);
	hardware.init(&clockCall);
	for(int i=0;i<6;i++){
	envelope[i].setAttackRate(1000);
	envelope[i].setDecayRate(2000);
	envelope[i].setReleaseRate(3000);
	envelope[i].setSustainLevel(0.5);
	envelope[i].setTargetRatioA(0.5);
	envelope[i].setTargetRatioDR(0.01);
	LFO[i].init();
	LFO[i].setFreq(0);
	}
}
uint32_t gateTime;
bool gate[6];

uint32_t _time;

#define TRIGGER_MODE 1
#define GATE_MODE 2
#define CV_MODE 3
#define INTERPOLATE_MODE 4
#define LFO_MODE 5
#define ADSR_MODE 6

void loop() {
	com.update();
	for(int i=0;i<6;i++){
		switch(channelMode[i]){
		case TRIGGER_MODE:
			break;
		case GATE_MODE:
			break;
		case CV_MODE:
			break;
		case INTERPOLATE_MODE:
			break;
		case LFO_MODE:
			break;
		case ADSR_MODE:
			if(hardware.getCVValue(i)>>9) gate[i]=1;
			else gate[i]=0;
			envelope[i].gate(gate[i]);
			break;
		}
	}

	/*
	 {
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
		LFO[i].step();
		LFO[i].getCurrentValue();
	}

	}
	_time=hardware.getElapsedBastlCycles();
*/



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


void reflectModeChange(uint8_t channel){
	switch(channelMode[channel]){
	case TRIGGER_MODE:
		break;
	case GATE_MODE:
		break;
	case CV_MODE:
		break;
	case INTERPOLATE_MODE:
		break;
	case LFO_MODE:
		break;
	case ADSR_MODE:
		break;
	}
}

void reflectValueChange(uint8_t channel, uint8_t value){
	switch(channelMode[channel]){
	case TRIGGER_MODE:
		break;
	case GATE_MODE:
		break;
	case CV_MODE:
		break;
	case INTERPOLATE_MODE:
		break;
	case LFO_MODE:
		break;
	case ADSR_MODE:
		break;
	}
}
