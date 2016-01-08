#include "../Headers/serial.h"

using namespace std;

/**
 * @brief [brief description]
 * @details [long description]
 */
Serial::Serial() : port("/dev/ttyO4")
{
	/// Open file descriptor.
	fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

	/// Check if file descriptor is open.
	if(fd < 0){
		cerr << "Serial port " << port << " unable to open." << endl;
	}
	else if(fd >= 0){
		cout << "Serial port " << port << " is open." << endl;

		/// Get current settings for the port.
		tcgetattr(fd, &settings);

		/// Set input and output baudrate.
		cfsetispeed(&settings, BAUDRATE);
		cfsetospeed(&settings, BAUDRATE);

		/// Makes sure that application does not become owner of the port.
		settings.c_cflag |= (CLOCAL | CREAD);

		/// Set new settings for the port.
		tcsetattr(fd, TCSANOW, &settings);
	}
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
 * @param msg [description]
 */
void Serial::send(string msg)
{
	/// Send string.
	int err = write(fd, msg.c_str(), msg.size());

	/// Check for errors while sending string.
	if(err < 0){
		cerr << "Unable to write." << endl;
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
