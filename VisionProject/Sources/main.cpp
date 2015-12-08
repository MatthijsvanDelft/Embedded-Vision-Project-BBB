  // Test to check the OpenCV version
  // Build on Linux with:
  
  #include "../Headers/Main.h"
  
 int main() {
 	
	GPIOHandler gpiohandler;
	Grabber grabber;
	DIP dip;

	cv::Mat *test_img;
	cv::Mat gray_img;

	gpiohandler.toggleAllLEDs();
	usleep(2000);
		
	test_img = grabber.getImage();
	gpiohandler.toggleLED(LEDRED);

	cv::cvtColor(*test_img, *test_img, CV_BGR2GRAY);

	dip.setSourceImage(test_img);

	dip.visionSet1();

	gray_img = *dip.getEnhancedImage();


	imwrite( "Frame.jpg", gray_img);	

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
