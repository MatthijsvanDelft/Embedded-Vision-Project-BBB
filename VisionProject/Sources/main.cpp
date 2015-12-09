// Main program Embedded Vision Project
// Created by Matthijs van Delft & Mike van Eerd
  
  #include "../Headers/Main.h"
  #include "../Headers/handler.h"
  
 int main() {
 	
	Handler handler;

    	try{
     	   handler.start();

   	}
    	catch(std::exception& e)
    	{
        	std::cerr << "Type: " << typeid(e).name() << "\tException: " << e.what() << std::endl;
    	}

/*
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
*/

 	return 0;
}
