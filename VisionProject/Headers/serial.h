#ifndef SERIAL_H
#define SERIAL_H

#include <iostream>
#include <string>
#include <fcntl.h> 
#include <unistd.h>

#include <stdio.h> 
#include <string.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <time.h> 

class Serial
{
public:
    Serial();
    ~Serial();
    void openPort();
    void send(std::string msg);
    void receive();

private:
    std::string port;
    int fd;

};

#endif // SERIAL_H
