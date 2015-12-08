#include "../Headers/testbench.h"

#define MILLI_SCALEFACTOR 1000
#define SEC_SCALEFACTOR 1000000

using namespace std;
using namespace chrono;

Testbench::Testbench()
{

}

/**
 * @brief Testbench::restartTimer
 */
void Testbench::restartTimer()
{
    /// Set begin time.
    beginTime = steady_clock::now();
}

/**
 * @brief Testbench::displayElapsedTime
 * @param msg
 */
void Testbench::displayElapsedTime(string msg)
{
    /// Calculates difference in begin time en time now in ns.
    std::stringstream ss;
    endTime = steady_clock::now();
    duration = duration_cast<nanoseconds>(endTime - beginTime);

    /// Scales time in ns to ms and prints to standart out.
    ss << "Message: " << msg << "\tPassed time: " << duration.count()*MILLI_SCALEFACTOR << "ms";
    cout << "test" << ss.str() << endl;
}

/**
 * @brief Testbench::getElapsedTime
 * @return elapsed time in ms.
 */
float Testbench::getElapsedTime()
{
    /// Calculates difference in begin time en time now in ns.
    endTime = steady_clock::now();
    duration = duration_cast<nanoseconds>(endTime - beginTime);

    /// Scales time in ns to ms and returns in float.
    return duration.count()*MILLI_SCALEFACTOR;
}

