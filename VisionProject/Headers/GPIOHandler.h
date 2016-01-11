#ifndef GPIOHANDLER_H
#define GPIOHANDLER_H

#define WIDTH 640
#define HEIGHT 480

#include "../Headers/SimpleGPIO.h"
#include "../Headers/GPinput.h"

/*! \class GPIOHANDLER class
    \brief destription of GPIOHANDLER class.
*/
class GPIOHandler
{
private:
	bool bLEDGREEN;	
	bool bLEDYELLOW;	
	bool bLEDRED;	

public:
	GPIOHandler();
	~GPIOHandler();

	int ReadAnalogIn();
	void setAllLEDsHigh();
	void setAllLEDsLow();
	void setLEDLow(unsigned int LEDColor);
	void setLEDHigh(unsigned int LEDColor);

	void toggleAllLEDs();
	void toggleLED(unsigned int LEDColor);

	int readBtns();

	const unsigned int LEDGREEN_GPIO = 60;   	// GPIO1_28 = (1x32) + 28 = 60
	const unsigned int LEDYELLOW_GPIO = 48;  	// GPIO1_16 = (1x32) + 16 = 48
	const unsigned int LEDRED_GPIO = 31;     	// GPIO0_31 = (0x32) + 31 = 31

	const unsigned int Button1_GPIO = 15;    	// GPIO0_15 = (0x32) + 15 = 15
	const unsigned int Button2_GPIO = 14;    	// GPIO0_14 = (0x32) + 14 = 14
	const unsigned int Button3_GPIO = 2;	 	// GPIO0_13 = (0x32) + 2 = 2

	
protected:


};

#endif // GPIOHANDLER_H
