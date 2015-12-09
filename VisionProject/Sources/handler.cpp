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
    determineTrackMask();
    determineFinishMask();
    threadGpio = thread(&Handler::readGpio, this);
    //threadSerial = thread(&Handler::displayInfo, this);
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
		subjectTestbench = "Grabber";
		testbench.restartTimer();

		test_img = grabber.getImage();
		//gpiohandler.toggleLED(LEDRED);
	 
		testbench.displayElapsedTime(subjectTestbench);
		//cv::cvtColor(*test_img, *test_img, CV_BGR2GRAY);
		//dip.setSourceImage(test_img);
		//dip.visionSet1();
		//gray_img = *dip.getEnhancedImage();
		//imwrite( "Frame.jpg", *test_img);

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
	int test = gpiohandler.readBtns();
        //cout << "Read GPIO" << "\tthread id:" << this_thread::get_id() << endl;
	if(test == 1){
		//cout << "Btn " << test << " is pushed" << endl;
		cout << "Program paused,   stop capturing frames" << endl;
		raceActive = false;
		gpiohandler.setLEDHigh(LEDRED); gpiohandler.setLEDLow(LEDGREEN);
	}

	if(test == 2){
		//cout << "Btn " << test << " is pushed" << endl;
		cout << "Program started, start capturing frames" << endl;
		raceActive = true;
		gpiohandler.setLEDLow(LEDRED); gpiohandler.setLEDHigh(LEDGREEN);
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
        cout << "Display info" << "\tthread id:" << this_thread::get_id() << endl;
        mtxSerial.unlock();
        this_thread::sleep_for(chrono::milliseconds(SERIAL_THREAD_DELAY_MS));
        }
}

/**
 * @brief Handler::determineTrackMask
 */
void Handler::determineTrackMask()
{

}

/**
 * @brief Handler::determineFinishMask
 */
void Handler::determineFinishMask()
{

}

/**
 * @brief Handler::determineCarStatus
 */
void Handler::determineCarStatus()
{

}


