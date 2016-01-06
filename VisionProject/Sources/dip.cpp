#include "../Headers/dip.h"

using namespace std;

/**
 * @brief DIP::DIP
 */
DIP::DIP()
{

}

/**
 * @brief DIP::visionSet1
 */
void DIP::visionSet1()
{
	threshold(src, &dst, 200, 255);
	//setSelected(&dst, &dst, 1, 255);
}

/**
 * @brief DIP::visionSet2
 */
void DIP::visionSet2()
{
	invert(&dst, &dst);
	//removeBorderBlobs(dst, dst, FOUR);
	contrastStretchFast(&dst, &dst);
	//setSelected(&dst, &dst, 1, 255);
}

/**
 * @brief DIP::visionSet3
 */
void DIP::visionSet3()
{
	threshold(src, &dst, 245, 255);
	removeBorderBlobs(&dst, &dst, EIGHT);    
}

/**
 * @brief DIP::contrastStretchFast
 * @param src, pointer to source image.
 * @param dst, pointer to desitination image.
 */
void DIP::contrastStretchFast(cv::Mat *src, cv::Mat *dst)
{
    register uint8_t *s = (uint8_t *)src->data;
    register uint8_t *d = (uint8_t *)dst->data;
    register uint32_t i = (src->rows * src->cols);
    register uint8_t lowestPixel = 255;
    register uint8_t highestPixel = 0;
    register uint8_t bottom = 0;
    register uint8_t top = 255;
    register float stretchFactor = 0;
    uint8_t lut[256];

    /// Reads pixel values and looks for lowest and highest pixel value.
    while(i > 0){        
        if(*s < lowestPixel) {lowestPixel = *s;}
        if(*s > highestPixel) {highestPixel = *s;}
        ++s;
        --i;
    }

    /// Calculates the stretch factor.
    stretchFactor = ((float)(top-bottom)/(highestPixel-lowestPixel));

    /// Creating a look-up-table
    i = 0;
    while(i < 256){
        lut[i] = (uint8_t)((i-lowestPixel)*stretchFactor+0.5f+bottom);
        ++i;
    }

    /// Initialize counters.
    s = (uint8_t *)src->data;
    d = (uint8_t *)dst->data;
    i = (src->rows * src->cols);

    /// Calulate new image with parameters and stretch factor.
    while(i > 0){
        *d = lut[*s];
        ++d;
        ++s;
        --i;
    }
}

/**
 * @brief DIP::threshold
 * @param src
 * @param dst
 * @param low
 * @param high
 */
void DIP::threshold(cv::Mat *src, cv::Mat *dst, uint8_t low, uint8_t high)
{
    register uint8_t *s = (uint8_t *)src->data;
    register uint8_t *d = (uint8_t *)dst->data;
    register uint32_t i = (src->rows * src->cols);
    uint8_t lut[256];

    /// Creating a look-up-table.
    i = 0;
    while(i < 256){
        if((i >= low) && (i <= high)) {lut[i] = 1;}
        else {lut[i] = 0;}
        ++i;
    }

    /// Initialize counter.
    i = (src->rows * src->cols);

    /// Reads pixel values, if value is between high and low make it 1, else 0.
    while(i > 0){
        *d = lut[*s];
        ++d;
        ++s;
        --i;
    }
}

/**
 * @brief DIP::histogram
 * @param im
 * @param hist
 * @param sum
 */
void DIP::histogram(cv::Mat *im, uint16_t *hist, uint32_t sum)
{
    /// Functioneert in QT.
    register uint8_t *s = (uint8_t *)im->data;
    register uint32_t i = 0;
    register uint16_t *histogram = hist;
    register uint32_t *sumHistogram = &sum;

    /// Initialize array with zero's.
    while(i < 256){
        *histogram = 0;
        ++histogram;
        ++i;
    }

    histogram = hist;
    i = (im->rows * im->cols);

    /// Create histogram
    while(i > 0){
        *(histogram + *s) += 1;
        *sumHistogram += *s;
        ++s;
        --i;
    }
}

/**
 * @brief DIP::thresholdIsoData
 * @param src
 * @param dst
 * @param brightness
 */
void DIP::thresholdIsoData(cv::Mat *src, cv::Mat *dst, uint8_t brightness)
{

    register uint32_t i = 0;
    uint16_t histogramArray[256];
    uint32_t sumHistogram = 0;
    register uint8_t lowestpixel = 255;
    register uint8_t highestpixel = 0;
    register uint8_t t0 = 0;
    register uint8_t t1 = 0;
    register uint32_t meanLeft = 0;
    register uint32_t meanRight = 0;
    register uint32_t nrPixelsLeft = 0;
    register uint32_t nrPixelsRight = 0;
    register uint8_t low = 0;
    register uint8_t high = 0;

    /// Call histogram function
    histogram(src, histogramArray, sumHistogram);


    /// Calculate highest and lowest pixel value
    while(i < 256){
        if(histogramArray[i] != 0){
            if(i < lowestpixel) {lowestpixel = i;}
            if(i > highestpixel) {highestpixel = i;}
        }
        ++i;
    }

    /// Calculate starting point for 2 means calculation
    t1 = (lowestpixel+highestpixel)/2;

    /// 2 means calculation
    while(t0 != t1){
        t0 = t1;
        i = nrPixelsLeft = nrPixelsRight = meanLeft = meanRight = 0;
        while(i < 256){
            if((i < t0) && (histogramArray[i] != 0)){
                nrPixelsLeft += histogramArray[i];
                meanLeft += i*histogramArray[i];
            }
            else if((i >= t0) && (histogramArray[i] != 0)){
                nrPixelsRight += histogramArray[i];
                meanRight += i*histogramArray[i];
            }
            if((i == 255) && (nrPixelsLeft != 0) && (nrPixelsRight != 0)){
                meanLeft = meanLeft/nrPixelsLeft;
                meanRight = meanRight/nrPixelsRight;
            }
            ++i;
        }
        t1 = (meanLeft+meanRight)/2;
    }

	std::cout << "Threshold isodata value: " << (int)t1 << std::endl;
    /// Determine threshold values
    if(brightness == DARK){
        low = 0;
        high = t1;
    }
    else if(brightness == BRIGHT){
        low = t1;
        high = 255;
    }

    /// Threshold
    threshold(src, dst, low, high);
}

/**
 * @brief DIP::invert
 * @param scr, pointer to source image.
 * @param dst, pointer to destination image.
 */
void DIP::invert(cv::Mat *src, cv::Mat *dst)
{
    register uint8_t *s = (uint8_t *)src->data;
    register uint8_t *d = (uint8_t *)dst->data;
    register uint32_t i = (src->rows * src->cols);
    uint8_t lut[255];

    /// Create look-up-table.
    i = 0;
    while(i < 255){
        lut[i] = 255-i;
        ++i;
    }

    /// Initialize counter.
    i = (src->rows * src->cols);

    /// Invert each pixel.
    while(i > 0){
        *d = lut[*s];
        ++s;
        ++d;
        i--;
    }
}

/**
 * @brief DIP::setSelected
 * @param src, pointer to source image.
 * @param dst, pointer to destination image.
 * @param selected, selected pixels with this value.
 * @param value, change selected pixels to this vlaue.
 */
void DIP::setSelected(cv::Mat *src, cv::Mat *dst, uint8_t selected, uint8_t value)
{
    register uint8_t *s = (uint8_t *)src->data;
    register uint8_t *d = (uint8_t *)dst->data;
    register uint32_t i = (src->rows * src->cols);

    /// Set selected pixels to value.
    while(i > 0){
        (*s == selected) ? *d = value : *d = *s;
        ++s;
        ++d;
        i--;
    }
}

/**
 * @brief DIP::multiply
 * @param src, pointer to source image.
 * @param dst, pointer to destination image.
 */
void DIP::multiply(cv::Mat *src, cv::Mat *dst)
{
    register uint8_t *s = (uint8_t *)src->data;
    register uint8_t *d = (uint8_t *)dst->data;
    register uint32_t i = (src->rows * src->cols);

    /// Add source and destination image.
    while(i > 0){
        if((*s * *d) > 255){
            *d = 255;
        }
        else{
            *d = *s * *d;
        }
        ++s;
        ++d;
        --i;
    }
}

/**
 * @brief DIP::removeBorderBlobs
 * @param src
 * @param dst
 * @param connected
 */
void DIP::removeBorderBlobs(cv::Mat *src, cv::Mat *dst, uint8_t connected) // FOUR | EIGHT
{
    register uint16_t y = 0;
    register uint16_t x = 0;
    register uint32_t i = 0;
    register uint16_t shift = 1;
    register uint8_t nrNeighbours = 0;
    register uint32_t connectedPixels = 0;

    /// Check for borderblobs on highest and lowest row.
    for(i=0; i<(uint16_t)src->cols; ++i){
        if(src->at<uint8_t>(0, i) == 1){
            dst->at<uint8_t>(0, i) = 2;
        }
        if(src->at<uint8_t>((src->rows-1), i) == 1){
            dst->at<uint8_t>((src->rows-1), i) = 2;
        }
    }

    /// Check for borderblobs on the most left and right colom.
    for(i=0; i<(uint16_t)src->rows; ++i){
        if(src->at<uint8_t>(i, 0) == 1){
            dst->at<uint8_t>(i, 0) = 2;
        }
        if(src->at<uint8_t>(i, (src->cols-1)) == 1){
           dst->at<uint8_t>(i, (src->cols-1)) = 2;
        }
    }

    /// Make every pixel connected to borderblob 2.
    do{
        connectedPixels = 0;
        for(y=shift; y<src->rows-shift; ++y){
            for(x=shift; x<src->cols-shift; ++x){
                if(dst->at<uint8_t>(y, x) == 1){
                    nrNeighbours = neighbourCount(dst, x, y, 2, connected);
                    if(nrNeighbours > 0){
                        dst->at<uint8_t>(y, x) = 2;
                        ++connectedPixels;
                    }
                }
            }
        }
        if(connectedPixels != 0){
           for(y=src->rows-shift; y>shift; --y){
               for(x=src->cols-shift; x>shift; --x){
                   if(dst->at<uint8_t>(y, x) == 1){
                       nrNeighbours = neighbourCount(dst, x, y, 2, connected);
                       if(nrNeighbours > 0){
                           dst->at<uint8_t>(y, x) = 2;
                           ++connectedPixels;
                       }
                   }
               }
           }
        }
        ++shift;
    }
    while(connectedPixels != 0);

    /// Set borderblobs to zero.
    setSelected(dst, dst, 2, 0);
}

/**
 * @brief DIP::fillHoles
 * @param src
 * @param dst
 * @param connected
 */
void DIP::fillHoles(cv::Mat *src, cv::Mat *dst, uint8_t connected)
{
    register uint16_t y = 0;
    register uint16_t x = 0;
    register uint32_t i = 0;
    register uint16_t shift = 1;
    register uint8_t nrNeighbours = 0;
    register uint32_t connectedPixels = 0;

    /// Check for background on highest and lowest row.
    for(i=0; i<(uint16_t)src->cols; ++i){
        if(src->at<uint8_t>(0, i) == 0){
            dst->at<uint8_t>(0, i) = 2;
        }
        if(src->at<uint8_t>((src->rows-1), i) == 0){
            dst->at<uint8_t>((src->rows-1), i) = 2;
        }
    }

    /// Check for background on the most left and right colom.
    for(i=0; i<(uint16_t)src->rows; ++i){
        if(src->at<uint8_t>(i, 0) == 0){
            dst->at<uint8_t>(i, 0) = 2;
        }
        if(src->at<uint8_t>(i, src->cols-1) == 0){
            dst->at<uint8_t>(i, src->cols-1) = 2;
        }
    }

    /// Make every pixel connected to background 2.
    do{
        connectedPixels = 0;
        for(y=shift; y<src->rows-shift; ++y){
            for(x=shift; x<src->cols-shift; ++x){
                if(dst->at<uint8_t>(y, x) == 0){
                    nrNeighbours = neighbourCount(dst, x, y, 2, connected);
                    if(nrNeighbours > 0){
                        dst->at<uint8_t>(y, x) = 2;
                        ++connectedPixels;
                    }
                }
            }
        }
        if(connectedPixels != 0){
           for(y=src->rows-shift; y>shift; --y){
               for(x=src->cols-shift; x>shift; --x){
                   if(dst->at<uint8_t>(y, x) == 0){
                       nrNeighbours = neighbourCount(dst, x, y, 2, connected);
                       if(nrNeighbours > 0){
                           dst->at<uint8_t>(y, x) = 2;
                           ++connectedPixels;
                       }
                   }
               }
           }
        }
        ++shift;
    }
    while(connectedPixels != 0);

    /// Set holes to one and background to zero.
    setSelected(dst, dst, 0, 1);
    setSelected(dst, dst, 2, 0);
}

/**
 * @brief DIP::binaryEdgeDetect
 * @param src
 * @param dst
 * @param connected
 */
void DIP::binaryEdgeDetect(cv::Mat *src, cv::Mat *dst, uint8_t connected)
{
    register uint16_t y = 0;
    register uint16_t x = 0;
    register uint8_t pixelValue = 0;
    register uint8_t neighbourCount = 0;

    /// Set a 2 in every pixel that is not connected to the background.
    for(y=0; y<src->rows; ++y){
        for(x=0; x<src->cols; ++x){
            pixelValue = dst->at<uint8_t>(y, x);
            if(pixelValue != 0){
                neighbourCount = neighboursEqualOrHigher(src, x, y, 1, connected);
                if(neighbourCount == (uint8_t)connected){
                    dst->at<uint8_t>(y, x) = 2;
                }
                else{
                    dst->at<uint8_t>(y, x) = 1;
                }
            }
        }
    }

    /// Set pixel with 2 to 0.
    setSelected(dst, dst, 2, 0);
}

/**
 * @brief DIP::neighbourCount
 * @param img
 * @param x
 * @param y
 * @param value
 * @param connected
 * @return
 */
uint8_t DIP::neighbourCount(cv::Mat *img, uint16_t x, uint16_t y, uint8_t value, uint8_t connected)
{
    /// Doesn't work for counting zeros.
    register uint8_t selectedValue = value;
    register uint8_t yMask = 0;
    register uint8_t xMask = 0;
    register uint8_t pixelValue = 0;
    register uint8_t neighbourCount = 0;
    register const uint8_t n = 3;
    uint8_t mask[n][n] = {};
    uint8_t maskFourConnected[n][n] = {{0, 1, 0},
                                       {1, 0, 1},
                                       {0, 1, 0}};
    uint8_t maskEightConnected[n][n] = {{1, 1, 1},
                                        {1, 0, 1},
                                        {1, 1, 1}};

    if(connected == FOUR){
        memcpy(&mask, &maskFourConnected, sizeof(mask));

    }
    else if(connected == EIGHT){
        memcpy(&mask, &maskEightConnected, sizeof(mask));
    }

    /// Calculate amount of neigbour pixels with value.
    for(yMask=0; yMask<n; ++yMask){
        for(xMask=0; xMask<n; ++xMask){
            pixelValue = img->at<uint8_t>((y+yMask-1), (x+xMask-1));
            if((y+yMask-1) < 0 || (y+yMask-1) >= img->rows){
                // cerr << "Out of image." << endl;
            }
            else if((x+xMask-1) < 0 || (x+xMask-1) >= img->cols){
                // cerr << "Out of image." << endl;
            }
            else if((mask[yMask][xMask]) && (pixelValue == value)){
                ++neighbourCount;
            }
        }
    }
    return neighbourCount;
}

/**
 * @brief DIP::neighboursEqualOrHigher
 * @param img
 * @param x
 * @param y
 * @param value
 * @param connected
 * @return
 */
uint8_t DIP::neighboursEqualOrHigher(cv::Mat *img, uint16_t x, uint16_t y, uint8_t value, uint8_t connected)
{
    register uint8_t yMask = 0;
    register uint8_t xMask = 0;
    register uint8_t pixelValue = 0;
    register uint8_t selectedValue = value;
    register uint8_t neighbourCount = 0;
    register const uint8_t n = 3;
    uint8_t mask[n][n] = {};
    uint8_t maskFourConnected[n][n] = {{0, 1, 0},
                                       {1, 0, 1},
                                       {0, 1, 0}};
    uint8_t maskEightConnected[n][n] = {{1, 1, 1},
                                        {1, 0, 1},
                                        {1, 1, 1}};

    if(connected == FOUR){
        memcpy(&mask, &maskFourConnected, sizeof(mask));

    }
    else if(connected == EIGHT){
        memcpy(&mask, &maskEightConnected, sizeof(mask));
    }

    /// Calculate amount of neigbour pixels with value.
    for(yMask=0; yMask<n; ++yMask){
        for(xMask=0; xMask<n; ++xMask){
            pixelValue = img->at<uint8_t>((y+yMask-1), (x+xMask-1));

            if((y+yMask-1) < 0 || (y+yMask-1) >= img->rows){
                // cerr << "Out of image." << endl;
            }
            else if((x+xMask-1) < 0 || (x+xMask-1) >= img->cols){
                // cerr << "Out of image." << endl;
            }
            else if((mask[yMask][xMask]) && (pixelValue >= value)){
                ++neighbourCount;
            }
        }
    }
    return neighbourCount;
}

/**
 * @brief DIP::setSourceImage
 * @param im, pointer to image.
 */
void DIP::setSourceImage(cv::Mat * im)
{
	src = im;
/// Give dst image same size as src image
	dst= *src;
}

/**
 * @brief DIP::getEnhancedImage
 * @return dst, pointer to enhanced image.
 */
cv::Mat * DIP::getEnhancedImage()
{
    return &dst;
}

