#ifndef HANDLER_H
#define HANDLER_H

#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <mutex>
#include "dip.h"
#include "Main.h"
#include "testbench.h"

#define ROUTINE_THREAD_DELAY_MS 0 //500
#define GPIO_THREAD_DELAY_MS 120
#define SERIAL_THREAD_DELAY_MS 500

class Handler
{
public:
    Handler();
    void start();
    void stop();
	
	cv::Mat *test_img;
	cv::Mat gray_img;

private:
    bool routineThreadActive;
    bool gpioThreadActive;
    bool serialThreadActive;
    bool raceActive;
    std::thread threadGpio;
    std::thread threadSerial;
    std::mutex mtxRoutine;
    std::mutex mtxGpio;
    std::mutex mtxSerial;
    std::string subjectTestbench;

    //DIP dipRoutine;
    GPIOHandler gpiohandler;
    Grabber grabber;
    DIP dip;
    Testbench testbench;

    void routine();
    void readGpio();
    void displayInfo();
    void determineTrackMask();
    void determineFinishMask();
    void determineCarStatus();
};

#endif // HANDLER_H

