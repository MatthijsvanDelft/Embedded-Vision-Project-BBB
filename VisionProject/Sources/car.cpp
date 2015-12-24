#include "../Headers/car.h"

#define MILLI_SCALEFACTOR 1000
#define SEC_SCALEFACTOR 1000000

using namespace std;
using namespace chrono;

/**
 * @brief Car::Car
 */
Car::Car()
{

}

/**
 * @brief Car::restartLapTimer
 */
void Car::restartLapTimer()
{
    /// Set begin time.
    beginTime = steady_clock::now();
}

/**
 * @brief Car::displayLapTime
 * @param msg
 */
void Car::displayLapTime(string *msg)
{
    /// Set end time.
    endTime = steady_clock::now();

    /// Calculates difference in begin time en time now in ns.
    std::stringstream ss;    
    lapDuration = duration_cast<nanoseconds>(endTime - beginTime);

    /// Scales time in ns to ms and prints to standart out.
    ss << "Message: " << *msg << "\tPassed time: " << lapDuration.count()*MILLI_SCALEFACTOR << "ms";
    cout << "test" << ss.str() << endl;
}

/**
 * @brief Car::getLapTime
 * @return
 */
float Car::getLapTime()
{
    /// Calculates difference in begin time en time now in ns.
    endTime = steady_clock::now();
    lapDuration = duration_cast<milliseconds>(endTime - beginTime);

    /// Scales time in ns to ms and returns in float.
    return lapDuration.count()*MILLI_SCALEFACTOR;
}

/**
 * @brief Car::setCoordinates
 * @param coor
 */
void Car::setCoordinates(const unsigned int xCoor, const unsigned int yCoor)
{
    posCoor.xCoordinate = xCoor;
    posCoor.yCoordinate = yCoor;
}

/**
 * @brief Car::getCoordinates
 * @return
 */
coordinates *Car::getCoordinates()
{
    return & posCoor;
}

/**
 * @brief Car::setNrRank
 * @param rank
 */
void Car::setNrRank(const unsigned int rank)
{
    nrRank = rank;
}

/**
 * @brief Car::getNrRank
 * @return
 */
unsigned int Car::getNrRank()
{
    return nrRank;
}

/**
 * @brief Car::setNrLaps
 * @param laps
 */
void Car::setNrLaps(const unsigned int laps)
{
    nrLaps = laps;
}

/**
 * @brief Car::getNrLaps
 * @return
 */
unsigned int Car::getNrLaps()
{
    return nrLaps;
}

/**
 * @brief Car::setTeam
 * @param team
 */
void Car::setTeam(const unsigned int team)
{
    nrTeam = team;
}

/**
 * @brief Car::getTeam
 * @return
 */
unsigned int Car::getTeam()
{
    return nrTeam;
}

/**
 * @brief Car::setSymbol
 * @param sym
 */
void Car::setSymbol(const std::string sym)
{
    symbol = sym;
}

/**
 * @brief Car::getSymbol
 * @return
 */
std::string Car::getSymbol()
{
    return symbol;
}

/**
 * @brief Car::setOnFinish
 * @param fin
 */
void Car::setOnFinish(const bool fin)
{
    onFinish = fin;
}

/**
 * @brief Car::getOnFinish
 * @return
 */
bool Car::getOnFinish()
{
    return onFinish;
}

/**
 * @brief Car::setOnTrack
 * @param track
 */
void Car::setOnTrack(const bool track)
{
    onTrack = track;
}

/**
 * @brief Car::getOnTrack
 * @return
 */
bool Car::getOnTrack()
{
    return onTrack;
}

/**
 * @brief Car::setDsqStatus
 * @param dsq
 */
void Car::setDsqStatus(const bool dsq)
{
    dsqStatus = dsq;
}

/**
 * @brief Car::getDsqStatus
 * @return
 */
bool Car::getDsqStatus()
{
    return dsqStatus;
}

