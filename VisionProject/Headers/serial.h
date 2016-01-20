#ifndef SERIAL_H
#define SERIAL_H

#include <iostream>
#include <string>
#include <fcntl.h> 
#include <unistd.h>
#include <termio.h>

#include <stdio.h> 
#include <string.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <time.h> 

#define BAUDRATE B115200

class Serial
{
public:
    Serial();
    ~Serial();
    void send(std::string *msg);
    void receive();

private:
    std::string port;
    struct termios settings;
    int fd;

};

#endif // SERIAL_H
