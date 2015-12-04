  // Test to check the OpenCV version
  // Build on Linux with:
  
  #include "../Headers/Main.h"
  
 int main() {
 	
	GPIOHandler gpiohandler;
	Grabber grabber;

	cv::Mat *test_img;
	
	gpiohandler.toggleAllLEDs();
	usleep(2000);

	for(int i = 0; i<5; i++){
		
		test_img = grabber.getImage();
		gpiohandler.toggleLED(LEDRED);
		}

	imwrite( "Frame.jpg", *test_img);	

	gpiohandler.toggleLED(LEDGREEN);
	usleep(500000);
	/*
	gpiohandler.setLEDHigh(LEDRED);
	
	std::cout << "AnalogValue = " << gpiohandler.ReadAnalogIn() << std::endl;
	usleep(2000000); 
	gpiohandler.setLEDHigh(LEDYELLOW);
	usleep(2000000); 
	gpiohandler.setLEDHigh(LEDGREEN);
	usleep(2000000);
	*/

 	return 0;
}
