#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

#warning ToDo: Rondetijden testen.

/**
 * @brief The coordinates struct
 */
struct coordinates{
    unsigned int xCoordinate;
    unsigned int yCoordinate;
};

/**
 * @brief The Car class
 */
class Car
{
public:
    Car();
    void restartLapTimer();
    void displayLapTime(std::string msg);
    float getLapTime();
    void setCoordinates(const unsigned int xCoor, const unsigned int yCoor);
    coordinates * getCoordinates();
    void setNrRank(const unsigned int rank);
    unsigned int getNrRank();
    void setNrLaps(const unsigned int laps);
    unsigned int getNrLaps();
    void setTeam(const unsigned int team);
    unsigned int getTeam();
    void setSymbol(const std::string sym);
    std::string getSymbol();
    void setOnFinish(const bool fin);
    bool getOnFinish();
    void setOnTrack(const bool track);
    bool getOnTrack();
    void setDsqStatus(const bool dsq);
    bool getDsqStatus();

private:
    std::chrono::steady_clock::time_point beginTime;
    std::chrono::steady_clock::time_point endTime;
    std::chrono::duration<float> lapDuration;
    coordinates posCoor;
    unsigned int nrRank;
    unsigned int nrLaps;
    unsigned int nrTeam;
    std::string symbol;
    bool onFinish;
    bool onTrack;
    bool dsqStatus;
};

#endif // CAR_H

