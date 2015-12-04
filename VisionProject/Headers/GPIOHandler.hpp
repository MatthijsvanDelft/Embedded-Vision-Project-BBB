#ifndef GPIOHANDLER_H
#define GPIOHANDLER_H

#define WIDTH 640
#define HEIGHT 480

#include "../Headers/SimpleGPIO.h"

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

	const unsigned int LEDGREEN_GPIO = 60;   // GPIO1_28 = (1x32) + 28 = 60
	const unsigned int LEDYELLOW_GPIO = 48;  // GPIO1_16 = (1x32) + 16 = 48
	const unsigned int LEDRED_GPIO = 31;     // GPIO0_31 = (0x32) + 31 = 31

	
protected:


};

#endif // GPIOHANDLER_H
