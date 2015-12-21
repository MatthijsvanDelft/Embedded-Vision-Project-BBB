#ifndef GRABBER_H
#define GRABBER_H

#define BUFF_SIZE 8
#define SLEEP_TIME 10000
#define FRAME_WIDTH 320 //640
#define FRAME_HEIGHT 240 //480

#include "../Headers/Main.h"
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <libv4l2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

/*! \class Grabber class
    \brief destription of Grabber class.
*/
class Grabber
{
private:
	void xioctl(int fh, int request, void *arg);

	int init_mmap();
	
	struct v4l2_format              fmt;
        struct v4l2_buffer              buf;
        struct v4l2_requestbuffers      req;
        enum v4l2_buf_type              type;
        fd_set                          fds;
        struct timeval                  tv;
	int                             r;
	int		 		fd;
        unsigned int                    i, n_buffers;
        const char                      *dev_name;
        char                            out_name[256];
        FILE                            *fout;

	struct buffer {
        	void   *start;
        	size_t length;
	};
	struct buffer                   *pBuffers;

public:
	Grabber();
	~Grabber();
	void readImage();
	
	cv::Mat mImage;
	cv::Mat *getImage();

protected:


};

#endif // GRABBER_H
