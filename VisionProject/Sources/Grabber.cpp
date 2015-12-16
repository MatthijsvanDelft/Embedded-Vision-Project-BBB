#include "../Headers/Grabber.hpp"


/** Grabber()
 *  \brief Constructor of Grabber
 */
Grabber::Grabber()
{
	mImage.create(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1);

	init_mmap();
	std::cout << "Init mmap done" << std::endl;
}

/** ~Grabber()
 *  \brief Destructor of Grabber
 */
Grabber::~Grabber()
{
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        xioctl(fd, VIDIOC_STREAMOFF, &type);
        for (i = 0; i < n_buffers; ++i)
                v4l2_munmap(pBuffers[i].start, pBuffers[i].length);
        v4l2_close(fd);
        
}	

/** Grabber::getImage()
 *  \return getImage() returns a pointer to the image file.
 */
cv::Mat *Grabber::getImage()
{
    readImage();
    return &mImage;
}

/** Grabber::readImage()
 *  \brief Read next image from camera
 */
void Grabber::readImage()
{
	do {
        	FD_ZERO(&fds);
                FD_SET(fd, &fds);

                /* Timeout. */
                tv.tv_sec = 2; 
                tv.tv_usec = 0;

                r = select(fd + 1, &fds, NULL, NULL, &tv);
                } while ((r == -1 && (errno = EINTR)));
	if (r == -1) {
                perror("select");
               	}
		
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        xioctl(fd, VIDIOC_DQBUF, &buf);
		
	///Set pointer to pixel channel in buffer

	uint8_t *pDataGrayscaleValue = (uint8_t *)pBuffers[buf.index].start;
	
	///put pixels in cv::mat, because the V4L2_PIX_FMT_YUYV format is used we only want the "Y" value. So pointer to the value has a sommation of the value 2
	for(int y = 0; y < mImage.rows; y++ ){
   		for(int x = 0; x < mImage.cols ; x++ ){
		mImage.at<uchar>(y,x) = *pDataGrayscaleValue;

		pDataGrayscaleValue+=2; 
      		}
    	}
	xioctl(fd, VIDIOC_QBUF, &buf);	//Reset buffer 
}

/** Grabber::xioctl()
 *  \brief Function to communicate with the V4L driver
 */
void Grabber::xioctl(int fh, int request, void *arg)
{
        int r;

        do {
                r = v4l2_ioctl(fh, request, arg);
        } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

        if (r == -1) {
                fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
                exit(EXIT_FAILURE);
        }
}

/** Grabber::init_mmap()
 *  \brief Function to set all the registers in the V4l driver
 */
int Grabber::init_mmap()
{
    	
	r= -1; fd = -1;
	dev_name = "/dev/video0";
        fd = v4l2_open(dev_name, O_RDWR | O_NONBLOCK, 0);
        if (fd < 0) {
                perror("Cannot open device");
                exit(EXIT_FAILURE);
        }

        CLEAR(fmt);
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width       = FRAME_WIDTH;
        fmt.fmt.pix.height      = FRAME_HEIGHT;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
        xioctl(fd, VIDIOC_S_FMT, &fmt);
        if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_YUYV ) {
                printf("Libv4l didn't accept grey format. Can't proceed.\n");
                exit(EXIT_FAILURE);
        }
	/*
        if ((fmt.fmt.pix.width != 640) || (fmt.fmt.pix.height != 480))
                printf("Warning: driver is sending image at %dx%d\n",
                        fmt.fmt.pix.width, fmt.fmt.pix.height);
	*/
        CLEAR(req);
        req.count = 1;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;
        xioctl(fd, VIDIOC_REQBUFS, &req);

        pBuffers = (buffer*)calloc(req.count, sizeof(buffer));

        for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
                CLEAR(buf);

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = n_buffers;

                xioctl(fd, VIDIOC_QUERYBUF, &buf);

		// pBuffers = pointer to buffer from V4L driver
                pBuffers[n_buffers].length = buf.length;
                pBuffers[n_buffers].start = v4l2_mmap(NULL, buf.length,
                              PROT_READ | PROT_WRITE, MAP_SHARED,
                              fd, buf.m.offset);

                if (MAP_FAILED == pBuffers[n_buffers].start) {
                        perror("mmap");
                        exit(EXIT_FAILURE);
                }
        }

	for (i = 0; i < n_buffers; ++i) {
                CLEAR(buf);
                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_MMAP;
                buf.index = i;
                xioctl(fd, VIDIOC_QBUF, &buf);
        }
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        xioctl(fd, VIDIOC_STREAMON, &type);

	return 1;
}


