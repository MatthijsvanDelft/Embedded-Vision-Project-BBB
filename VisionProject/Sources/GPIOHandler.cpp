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
 *  \brief Constructor of GPIOHandler
 */
GPIOHandler::GPIOHandler()
{
  	gpio_export(LEDGREEN_GPIO);
  	gpio_export(LEDYELLOW_GPIO);
 	gpio_export(LEDRED_GPIO);

  	gpio_set_dir(LEDGREEN_GPIO, OUTPUT_PIN);
  	gpio_set_dir(LEDYELLOW_GPIO, OUTPUT_PIN);
  	gpio_set_dir(LEDRED_GPIO, OUTPUT_PIN);
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
/** setLEDLow()
 *  \brief Set led low
 */
void GPIOHandler::setLEDLow(int LEDColor){

	gpio_set_value(LEDColor, LOW);

}

/** setLEDHigh()
 *  \brief Set led high
 */
void GPIOHandler::setLEDHigh(int LEDColor){

	gpio_set_value(LEDColor, HIGH);

}







/*
int readOut(std::istream& inStr)
{
  int retVal;
  inStr >> retVal;
  return retVal;
}

int main(int argc, char* argv[])
{
  // ncurses
  initscr();
  cbreak();
  noecho();

  std::ostringstream buff;

  int avgAIN3[BUFF_SIZE] = {0};
  int avgAIN5[BUFF_SIZE] = {0};

  int idx = 0;

  while (true)
  {
    try
    {
      if (++idx == BUFF_SIZE)
      {
        idx = 0;
      }

      std::ifstream AIN3 ("/sys/devices/ocp.3/helper.15/AIN3");
      std::ifstream AIN5 ("/sys/devices/ocp.3/helper.15/AIN5");

      avgAIN3[idx] = readOut(AIN3);
      avgAIN5[idx] = readOut(AIN5);
      int avg3 = 0;
      int avg5 = 0;
      for (std::size_t i = 0; i < BUFF_SIZE; ++i)
      {
        avg3 += avgAIN3[i];
        avg5 += avgAIN5[i];
      }
      avg3 /= BUFF_SIZE;
      avg5 /= BUFF_SIZE;

      buff.str("");
      buff.clear();
      buff << "AIN3 = " << std::setw(5) << avg3 << '\n';
      mvaddstr(1, 1, buff.str().c_str());
      buff.str("");
      buff.clear();
      buff << "AIN5 = " << std::setw(5) << avg5 << '\n';
      mvaddstr(2, 1, buff.str().c_str());
      usleep(SLEEP_TIME);
      refresh();
    }
    catch (...)
    {
      //std::cout << "\n\n";:
      usleep(SLEEP_TIME);
    }
  }
  endwin();

  return 0;
}*/
