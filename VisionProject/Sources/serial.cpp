#include "../Headers/serial.h"

using namespace std;

/**
 * @brief [brief description]
 * @details [long description]
 */
Serial::Serial() : port("/dev/ttyO1")
{
	openPort();
}

/**
 * @brief [brief description]
 * @details [long description]
 * @return [description]
 */
Serial::~Serial() 
{
	/// Close file descriptor.
	close(fd);
}

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param port [description]
 */
void Serial::openPort()
{
	/// Open file descriptor.
	fd = open(port.c_str(), O_RDWR);

	/// Check if file descriptor is open.
	if(fd == -1){
		cerr << "port " << port << " unable to open." << endl;
	}
	else if(fd >= 0){
		cout << "port " << port << " is open." << endl;
	}
}

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param msg [description]
 */
void Serial::send(string msg)
{
	#warning todo: test serial::receive
	/// Send string.
	int err = write(fd, msg.c_str(), msg.size());

	/// Check for errors while sending string.
	if(err == -1){
		cerr << "Unable to write." << endl;
	}
	else{
		cout << "Send message:" << msg << endl;
	}
}

/**
 * @brief [brief description]
 * @details [long description]
 */
void Serial::receive()
{
	#warning todo: serial::receive
}
