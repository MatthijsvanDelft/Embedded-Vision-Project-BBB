#include "../Headers/handler.h"

using namespace std;

Handler::Handler() : routineThreadActive(true), gpioThreadActive(true), serialThreadActive(true), raceActive(false)
{
	
}

/**
 * @brief Handler::start
 */
void Handler::start()
{
    gpiohandler.setLEDHigh(LEDRED);
    determineTrackMask();
    determineFinishMask();
    threadGpio = thread(&Handler::readGpio, this);
    threadSerial = thread(&Handler::displayInfo, this);
    routine();
}

/**
 * @brief Handler::stop
 */
void Handler::stop()
{
    gpioThreadActive = false;
    serialThreadActive = false;

    if(threadGpio.joinable()){
        threadGpio.join();
        cout << "threadGPIO joined" << endl;
    }
    if(threadSerial.joinable()){
        threadSerial.join();
        cout << "threadSerial joined" << endl;
    }
}

/**
 * @brief Handler::routine
 */
void Handler::routine()
{    
    while(routineThreadActive){
		if(raceActive){
			mtxRoutine.lock();
			//cout << "Routine" << "\tthread id:" << this_thread::get_id() << endl;
			// test_img = grabber.getImage();
			// gray_img = *test_img;		

			testbench.restartTimer();
			gray_img = cv::imread("Images/test1.png", CV_LOAD_IMAGE_GRAYSCALE);

			
			dip.setSourceImage(&gray_img);
			dip.visionSet3();
			gray_img = *dip.getEnhancedImage();

			// cv::imwrite( "Images/routine.png", gray_img );		

			classifier.setSourceImage(&gray_img);
			classifier.classifyCars();

			gpiohandler.toggleLED(LEDGREEN);

			string message = "Routine";
			testbench.displayElapsedTime(&message);

			mtxRoutine.unlock();
		}
        this_thread::sleep_for(chrono::milliseconds(ROUTINE_THREAD_DELAY_MS));
    }
}

/**
 * @brief Handler::readGpio
 */
void Handler::readGpio()
{
    while(gpioThreadActive){
        mtxGpio.lock();
		unsigned int btnIndex = gpiohandler.readBtns();
        //cout << "Read GPIO" << "\tthread id:" << this_thread::get_id() << endl;
		if(btnIndex == 1){
			cout << "Program paused,   stop capturing frames" << endl;
			raceActive = false;
			gpiohandler.setLEDHigh(LEDRED); gpiohandler.setLEDLow(LEDGREEN);
		}

		else if(btnIndex == 2){
			cout << "Program started, start capturing frames" << endl;
			raceActive = true;
			gpiohandler.setLEDLow(LEDRED); gpiohandler.setLEDHigh(LEDGREEN);
		}
		else if(btnIndex == 3){
			determineTrackMask();
			determineFinishMask();
			cout << "New TrackMask & FinishMask is created" << endl;
		}
      	mtxGpio.unlock();
        this_thread::sleep_for(chrono::milliseconds(GPIO_THREAD_DELAY_MS));
    }
}

/**
 * @brief Handler::displayInfo
 */
void Handler::displayInfo()
{
    while(serialThreadActive){
        mtxSerial.lock();
        // cout << "Display info" << "\tthread id:" << this_thread::get_id() << endl;
        mtxSerial.unlock();
        this_thread::sleep_for(chrono::milliseconds(SERIAL_THREAD_DELAY_MS));
        }
}

/**
 * @brief Handler::determineTrackMask
 */
void Handler::determineTrackMask()
{
	gpiohandler.setLEDHigh(LEDYELLOW);
	track_img = cv::imread("Images/inputRacetrack.png", CV_LOAD_IMAGE_GRAYSCALE);
	
	dip.setSourceImage(&track_img);

	dip.visionSet1();

	track_img = *dip.getEnhancedImage();

	cv::imwrite( "Bineary trackmask.png", track_img);
}

/**
 * @brief Handler::determineFinishMask
 */
void Handler::determineFinishMask()
{

	dip.visionSet2();	

	finish_img = *dip.getEnhancedImage();

	cv::imwrite( "Bineary finishmask.png", finish_img);
	gpiohandler.setLEDLow(LEDYELLOW);
}

/**
 * @brief Handler::determineCarStatus
 */
void Handler::determineCarStatus()
{

}


