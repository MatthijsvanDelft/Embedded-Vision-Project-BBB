#ifndef HANDLER_H
#define HANDLER_H

#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <mutex>
#include "GPIOHandler.h"
#include "grabber.h"
#include "dip.h"
#include "main.h"
#include "testbench.h"
#include "classifier.h"
#include "serial.h"

#define ROUTINE_THREAD_DELAY_MS 0
#define GPIO_THREAD_DELAY_MS 120
#define SERIAL_THREAD_DELAY_MS 500

class Handler
{
public:
    Handler();
    void start();
    void stop();

private:
    bool routineThreadActive;
    bool gpioThreadActive;
    bool serialThreadActive;
    bool raceActive;
    bool dataReady;
    std::thread threadGpio;
    std::thread threadSerial;
    std::mutex mtxRoutine;
    std::mutex mtxGpio;
    std::mutex mtxSerial;
    std::string subjectTestbench;

    cv::Mat *sample_img;
    cv::Mat track_img;
    cv::Mat finish_img;
    std::vector<Car> *carVector;

    GPIOHandler gpiohandler;
    Grabber grabber;
    DIP dip;
    DIP dipTrackMask;
    DIP dipFinishMask;
    Serial serial;
    Testbench testbench;
    Classifier classifier;

    void routine();
    void readGpio();
    void displayInfo();
    void determineTrackMask();
    void determineFinishMask();
    void determineCarStatus();
    void determineCarRanks();
    std::string carToJSON(Car *car);
};

#endif // HANDLER_H

