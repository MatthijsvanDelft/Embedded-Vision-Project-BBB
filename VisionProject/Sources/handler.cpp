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
			// testbench.restartTimer();

			sample_img = grabber.getImage();
			cv::imwrite("Original.png", *sample_img);					
			
			dip.setSourceImage(sample_img);
			dip.visionSet3();
			sample_img = dip.getEnhancedImage();
			cv::imwrite("Enhanced.png", *sample_img);							

			classifier.setSourceImage(sample_img);
			classifier.classifyCars();
			carVector = classifier.getCarVector();
			dataReady = true;

			determineCarStatus();

			gpiohandler.toggleLED(LEDGREEN);

			string message = "Routine";
			// testbench.displayElapsedTime(&message);

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
			cout << "Program paused, stop capturing frames" << endl;
			raceActive = false;
			gpiohandler.setLEDHigh(LEDRED);
			gpiohandler.setLEDLow(LEDGREEN);
		}
		else if(btnIndex == 2){
			cout << "Program started, start capturing frames" << endl;
			raceActive = true;
			gpiohandler.setLEDLow(LEDRED); 
			gpiohandler.setLEDHigh(LEDGREEN);
		}
		else if(btnIndex == 3){
			gpiohandler.setLEDHigh(LEDYELLOW);
			determineTrackMask();
			determineFinishMask();
			gpiohandler.setLEDLow(LEDYELLOW);			
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
	coordinates *carPosition;
	string jsonInfo;

    while(serialThreadActive){
    	if(dataReady){
	        mtxSerial.lock();
	        // cout << "Display info" << "\tthread id:" << this_thread::get_id() << endl;

	        for(int i=0; i<(int)carVector->size(); ++i){	        
				carPosition = carVector->at(i).getCoordinates();
	        	if ((carPosition->xCoordinate != 0) && (carPosition->yCoordinate != 0)){
					jsonInfo = carToJSON(&(carVector->at(i)));
					serial.send(&jsonInfo);
	        	}
	        }

	        mtxSerial.unlock();
    	}
		this_thread::sleep_for(chrono::milliseconds(SERIAL_THREAD_DELAY_MS));
    }
}

/**
 * @brief Handler::determineTrackMask
 */
void Handler::determineTrackMask()
{
	track_img = *grabber.getImage();
	dipTrackMask.setSourceImage(&track_img);
	dipTrackMask.visionSet1();
	track_img = *dipTrackMask.getEnhancedImage();

	cv::imwrite("TrackMask.png", track_img);

	cout << "Track mask is created succesfully" << endl;
}

/**
 * @brief Handler::determineFinishMask
 */
void Handler::determineFinishMask()
{
	finish_img = cv::imread("TrackMask.png", CV_LOAD_IMAGE_GRAYSCALE);

	dipFinishMask.setSourceImage(&finish_img);

	dipFinishMask.visionSet2();	
	finish_img = *dipFinishMask.getEnhancedImage();
	cv::imwrite("FinishMask.png", finish_img);

	Classifier finishClassifier;
	finishClassifier.setSourceImage(&finish_img);
	finishClassifier.classifyFinish();

	finish_img = *finishClassifier.getLabeldImage();
	finish_t *finish = finishClassifier.getFinish();
	cout << "finish label:" << int(finish->blobLabel) << "\tinvarianceMoment1:" << finish->invarianceMoment1 <<  endl;
	
	/// Check is finish label is not zero.
	if (int(finish->blobLabel)){
		cout << "Finish mask is created succesfully" << endl;
	}
	else{
		cerr << "Finish mask is not created" <<  endl;
	}
}

/**
 * @brief Handler::determineCarStatus
 */
void Handler::determineCarStatus()
{
	coordinates *carPosition;
	for (int i = 0; i < (int)carVector->size(); ++i)
	{
		carPosition = carVector->at(i).getCoordinates();
		float lapTime = 0;	

		/// Check if car is on the track.
		if(track_img.at<uint8_t>(carPosition->yCoordinate, carPosition->xCoordinate)){
			carVector->at(i).setOnTrack(true);
		}
		else{
			carVector->at(i).setOnTrack(false);
			carVector->at(i).setDsqStatus(true);
		}

		/// Check if car is on at the finish.	
		if(finish_img.at<uint8_t>(carPosition->yCoordinate, carPosition->xCoordinate) == 1){
			if(carVector->at(i).getOnFinish() == false){
				
				/// Get lap time and restart lap timer for next lap.
				lapTime = carVector->at(i).getLapTime();
				carVector->at(i).restartLapTimer();
				cout << "Car:" << i+1 << "\tSymbol:" << carVector->at(i).getSymbol() << "\tLap time:" << lapTime << endl;

				/// Add a lap.
				carVector->at(i).setNrLaps(carVector->at(i).getNrLaps() + 1);
			}
			carVector->at(i).setOnFinish(true);
		}
		else{
			carVector->at(i).setOnFinish(false);
		}
		// cout << "Car:" << i+1 << "\tSymbol:" << carVector->at(i).getSymbol() << "\tCentoid:(" << carPosition->xCoordinate << "," << carPosition->yCoordinate << ")"  << "\tOn track:" << carVector->at(i).getOnTrack() << "\tOn finish:" << carVector->at(i).getOnFinish() << "\tLap time:" << lapTime << endl;
	}
}

/**
 * @brief [brief description]
 * @details [long description]
 */
void Handler::determineCarRanks()
{

}

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param car [description]
 * @return [description]
 */
string Handler::carToJSON(Car *car)
{
	stringstream ssJsonInfo;

	coordinates *coors = car->getCoordinates();

	/// Create JSON string.
	// ssJsonInfo << "{\"team\":\"" << car->getTeam() << "\"," ;
	ssJsonInfo << "{\"symbol\":\"" << car->getSymbol() << "\"," ;
	ssJsonInfo << "\"rank\":\"" << car->getNrRank() << "\"," ;
	ssJsonInfo << "\"laps\":\"" << car->getNrLaps() << "\"," ;
	ssJsonInfo << "\"coordinate x\":\"" << coors->xCoordinate << "\"," ;
	ssJsonInfo << "\"coordinate y\":\"" << coors->yCoordinate << "\"," ;
	ssJsonInfo << "\"lap time\":\"" << car->getLapTime() << "\"," ;
	ssJsonInfo << "\"on finish\":\"" << car->getOnFinish() << "\"," ;
	ssJsonInfo << "\"on track\":\"" << car->getOnTrack() << "\"," ;
	ssJsonInfo << "\"disqualification\":\"" << car->getDsqStatus() << "\"}\n" ;

	return ssJsonInfo.str();
}
