#ifndef TESTBENCH_H
#define TESTBENCH_H

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

/**
 * @brief The Testbench class
 */
class Testbench
{
public:
    Testbench();
    void restartTimer();
    void displayElapsedTime(std::string msg);
    float getElapsedTime();

private:
    std::chrono::steady_clock::time_point beginTime;
    std::chrono::steady_clock::time_point endTime;
    std::chrono::duration<float> duration;
};

#endif // TESTBENCH_H

