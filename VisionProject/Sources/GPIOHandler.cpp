// Shows AIN3 and AIN5 values in sample loop.
// Averages inout signals for supressing some noise.
// Uses ncurses.

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <ncurses.h>
#include <unistd.h>
#include "../Headers/GPIOHandler.hpp"

#define BUFF_SIZE 8
#define SLEEP_TIME 10000

/** GPIOHandler()
 *  \brief Constructor of GPIOHandler, GPIO pins are high on startup
 */
GPIOHandler::GPIOHandler()
{
  	gpio_export(LEDGREEN_GPIO);
  	gpio_export(LEDYELLOW_GPIO);
 	gpio_export(LEDRED_GPIO);

  	gpio_set_dir(LEDGREEN_GPIO, OUTPUT_PIN);
  	gpio_set_dir(LEDYELLOW_GPIO, OUTPUT_PIN);
  	gpio_set_dir(LEDRED_GPIO, OUTPUT_PIN);

	bLEDGREEN = TRUE;	
	bLEDYELLOW = TRUE;	
	bLEDRED = TRUE;

}

/** ~GPIOHandler()
 *  \brief Destructor of GPIOHandler
 */
GPIOHandler::~GPIOHandler()
{
	gpio_unexport(LEDGREEN_GPIO);
  	gpio_unexport(LEDYELLOW_GPIO);
  	gpio_unexport(LEDRED_GPIO);
}	

/** ReadAnalogIn()
 *  \brief Read the value of the AnalogIn pins. (AIN3 & AIN5)
 */
int GPIOHandler::ReadAnalogIn()
{
	int AnalogValue;

	std::ifstream AIN3 ("/sys/devices/ocp.3/helper.15/AIN3");
	
  	AIN3 >> AnalogValue;

	return AnalogValue; 
}

/************************************* Set *************************************/

/** setAllLEDsHigh()
 *  \brief Set all LED's high
 */
void GPIOHandler::setAllLEDsHigh(){

	gpio_set_value(LEDGREEN_GPIO, HIGH);
	gpio_set_value(LEDYELLOW_GPIO, HIGH);
	gpio_set_value(LEDRED_GPIO, HIGH);

	bLEDGREEN = TRUE;	
	bLEDYELLOW = TRUE;	
	bLEDRED = TRUE;
}

/** setAllLEDsLow()
 *  \brief Set all LED's low
 */
void GPIOHandler::setAllLEDsLow(){

	gpio_set_value(LEDGREEN_GPIO, LOW);
	gpio_set_value(LEDYELLOW_GPIO, LOW);
	gpio_set_value(LEDRED_GPIO, LOW);

	bLEDGREEN = FALSE;	
	bLEDYELLOW = FALSE;	
	bLEDRED = FALSE;
}

/** setLEDLow()
 *  \brief Set LED low
 *  \arg int LEDColor
 */
void GPIOHandler::setLEDLow(unsigned int LEDColor){

	gpio_set_value(LEDColor, LOW);
	if(LEDColor == LEDGREEN_GPIO){		bLEDGREEN = FALSE;}
	else if(LEDColor == LEDYELLOW_GPIO){	bLEDYELLOW = FALSE;}
	else if(LEDColor == LEDRED_GPIO){	bLEDRED = FALSE;}
}

/** setLEDHigh()
 *  \brief Set led high
 *  \arg int LEDColor
 */
void GPIOHandler::setLEDHigh(unsigned int LEDColor){

	gpio_set_value(LEDColor, HIGH);
	if(LEDColor == LEDGREEN_GPIO){		bLEDGREEN = TRUE;}
	else if(LEDColor == LEDYELLOW_GPIO){	bLEDYELLOW = TRUE;}
	else if(LEDColor == LEDRED_GPIO){	bLEDRED = TRUE;}
}

/********************************** End Set ***********************************/

/*********************************** Toggle ***********************************/

/** toggleAllLEDs()
 *  \brief toggle all LED's
 */
void GPIOHandler::toggleAllLEDs(){	

	if(bLEDGREEN == TRUE){ 
		bLEDGREEN = FALSE; 
		gpio_set_value(LEDGREEN_GPIO, LOW);
	}
	else{
		bLEDGREEN = TRUE; 
		gpio_set_value(LEDGREEN_GPIO, HIGH);
	}

	if(bLEDYELLOW == TRUE){ 
		bLEDYELLOW = FALSE; 
		gpio_set_value(LEDYELLOW_GPIO, LOW);
	}
	else{
		bLEDYELLOW = TRUE; 
		gpio_set_value(LEDYELLOW_GPIO, HIGH);
	}

	if(bLEDRED == TRUE){ 
		bLEDRED = FALSE; 
		gpio_set_value(LEDRED_GPIO, LOW);
	}
	else{
		bLEDRED = TRUE; 
		gpio_set_value(LEDRED_GPIO, HIGH);
	}
}

/** toggleLED()
 *  \brief toggle LED
 *  \arg int LEDColor
 */
void GPIOHandler::toggleLED(unsigned int LEDColor){
	if(LEDColor == LEDGREEN_GPIO){
		if(bLEDGREEN == TRUE){ 
			bLEDGREEN = FALSE; 
			gpio_set_value(LEDGREEN_GPIO, LOW);
		}
		else{
			bLEDGREEN = TRUE; 
			gpio_set_value(LEDGREEN_GPIO, HIGH);
		}
	}

	if(LEDColor == LEDYELLOW_GPIO){
		if(bLEDYELLOW == TRUE){ 
			bLEDYELLOW = FALSE; 
			gpio_set_value(LEDYELLOW_GPIO, LOW);
		}
		else{
			bLEDYELLOW = TRUE; 
			gpio_set_value(LEDYELLOW_GPIO, HIGH);
		}
	}

	if(LEDColor == LEDRED_GPIO){
		if(bLEDRED == TRUE){ 
			bLEDRED = FALSE; 
			gpio_set_value(LEDRED_GPIO, LOW);
		}
		else{
			bLEDRED = TRUE; 
			gpio_set_value(LEDRED_GPIO, HIGH);
		}
	}

}

/******************************** End Toggle *********************************/

