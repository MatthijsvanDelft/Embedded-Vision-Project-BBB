  // Test to check the OpenCV version
  // Build on Linux with:
  
  #include <opencv2/opencv.hpp>
  #include <iostream>
  #include <unistd.h>
  #include "../Headers/GPIOHandler.hpp"

// LEDs
const unsigned int LEDGREEN = 60;   // GPIO1_28 = (1x32) + 28 = 60
const unsigned int LEDYELLOW = 48;  // GPIO1_16 = (1x32) + 16 = 48
const unsigned int LEDRED = 31;     // GPIO0_31 = (0x32) + 31 = 31
  
 int main() {
 	
	GPIOHandler gpiohandler;
	

 	std::cout << "Hello, OpenCV version "<< CV_VERSION << std::endl;	
	
	gpiohandler.setLEDHigh(LEDRED);
	
	std::cout << "AnalogValue = " << gpiohandler.ReadAnalogIn() << std::endl;
	usleep(2000000); 
	gpiohandler.setLEDHigh(LEDYELLOW);
	usleep(2000000); 
	gpiohandler.setLEDHigh(LEDGREEN);
	usleep(2000000);
	

 	return 0;
}
