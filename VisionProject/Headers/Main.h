#ifndef MAIN_H
#define MAIN_H 

#include<opencv2/opencv.hpp>

#include <iostream>
#include <unistd.h>
#include "../Headers/GPIOHandler.hpp"
#include "../Headers/Grabber.hpp"

// LEDs
const unsigned int LEDGREEN = 60;   // GPIO1_28 = (1x32) + 28 = 60
const unsigned int LEDYELLOW = 48;  // GPIO1_16 = (1x32) + 16 = 48
const unsigned int LEDRED = 31;     // GPIO0_31 = (0x32) + 31 = 31


#endif // MAIN_H
