#include "../Headers/classifier.h"

using namespace std;

Classifier::Classifier()
{
    /// Initialize car vector for 6 cars.
    carVector.resize(6);
}

/**
 * @brief Classifier::classifyCars
 */
void Classifier::classifyCars()
{
    uint8_t nrBlobs = 0;
    nrBlobs = labelBlobs(src, &dst, EIGHT);
    cout << "Blob count:" << int(nrBlobs) << endl;

    /// Resize vector.
    blobVector.resize(nrBlobs);

    blobAnalyse(&dst, nrBlobs);

	for(int i=0; i < (int)nrBlobs; i++){	    	
		cout << "Blob:" << i+1 << "\tForm factor:" << blobVector.at(i).formFactor << endl;
        cout << "Blob:" << i+1 << "\tX centroid:" << blobVector.at(i).xCentroid << endl;
        cout << "Blob:" << i+1 << "\tY centroid:" << blobVector.at(i).yCentroid << endl;
        cout << "Blob:" << i+1 << "\tInvariance moment::" << blobVector.at(i).invarianceMoment1 << endl;
	}

    /// Clear data for next frame.
    blobVector.clear();
}

/**
 * @brief Classifier::labelBlobs
 * @param src
 * @param dst
 * @param connected
 * @return
 */
uint32_t Classifier::labelBlobs(cv::Mat * src, cv::Mat * dst, uint8_t connected)
{
        register uint16_t y = 0;
        register uint16_t x = 0;
        register uint16_t shift = 0;
        register uint16_t blobCount = 1;
        register uint8_t pixelValue = 0;
        register uint8_t lowestNeighbour = 255;
        register uint16_t changeCount = 0;

        /// Set binary image to 255.
        setSelected(src, dst, 1, 255);

        /// Connects every pixel with the same(lowest) value.
        do{
            changeCount=0;
            for(y=0+shift; y<dst->rows-shift-1; y++){
                for(x=0+shift; x<dst->cols-shift-1; x++){
                    pixelValue = dst->at<uint8_t>(y, x);
                    if(pixelValue != 0)
                    {
                        lowestNeighbour = neighboursLowest(dst, x, y, connected);
                        if((pixelValue != 0) && (lowestNeighbour < pixelValue)){
                            dst->at<uint8_t>(y, x) = lowestNeighbour;
                            changeCount++;
                        }
                        else if(pixelValue == 255){
                            dst->at<uint8_t>(y, x) = blobCount;
                            blobCount++;
                            changeCount++;
                        } 
                    }
                }
            }
            if(changeCount != 0){
                changeCount=0;
                for(y=dst->rows-shift-1; y>0+shift; y--){
                    for(x=dst->cols-shift-1; x>0+shift; x--){
                        pixelValue = dst->at<uint8_t>(y, x);
                        if(pixelValue)
                        {
                            lowestNeighbour = neighboursLowest(dst, x, y, connected);
                            if((pixelValue != 0) && (lowestNeighbour < pixelValue)){
                                dst->at<uint8_t>(y, x) = lowestNeighbour;
                                changeCount++;
                            }
                            else if(pixelValue == 255){
                                dst->at<uint8_t>(y, x) = blobCount;
                                blobCount++;
                                changeCount++;
                            }    
                        }                        
                    }
                }
            }
            shift++;
        }
        while(changeCount != 0);

        /// rearrange labels.
        blobCount=1;
        for(y=0; y<dst->rows-1; y++){
            for(x=0; x<dst->cols-1; x++){
                pixelValue = dst->at<uint8_t>(y, x);
                if((pixelValue !=0) && (pixelValue >= blobCount)){
                    setSelected(dst, dst, pixelValue, blobCount);
                    blobCount++;
                }
            }
        }

        return blobCount-1;
}

/**
 * @brief Classifier::blobAnalyse
 * @param img 
 * @param blobcount
 */
void Classifier::blobAnalyse(cv::Mat *img, const uint8_t blobcount)
{
    register uint16_t y = 0;
    register uint16_t x = 0;
    register uint8_t i = 0;
    register uint8_t pixelValue = 0;
    register uint8_t borderingSides = 0;

    /// Calculate number of pixels, highest and most left positioned pixel for each blob.
    for(y=0; y<img->rows-1; y++){
        for(x=0; x<img->cols-1; x++){
            pixelValue = img->at<uint8_t>(y, x);
            if((pixelValue != 0) && (pixelValue <= blobcount)){
                blobVector.at(pixelValue-1).nof_pixels += 1;

                /// Check for highest and lowest positioned pixel of a blob. 
                if(blobVector.at(pixelValue-1).mostUpperPixel == 0) {blobVector.at(pixelValue-1).mostUpperPixel = y;}
                if(blobVector.at(pixelValue-1).mostLowerPixel == 0) {blobVector.at(pixelValue-1).mostLowerPixel = y;}
                else if(y > blobVector.at(pixelValue-1).mostLowerPixel) {blobVector.at(pixelValue-1).mostLowerPixel = y;}

                /// Check for most left and most right positioned pixel of a blob.
                if(blobVector.at(pixelValue-1).mostLeftPixel == 0) {blobVector.at(pixelValue-1).mostLeftPixel = x;}
                else if(x < blobVector.at(pixelValue-1).mostLeftPixel) {blobVector.at(pixelValue-1).mostLeftPixel = x;}
                if(blobVector.at(pixelValue-1).mostRightPixel == 0) {blobVector.at(pixelValue-1).mostRightPixel = x;}
                else if(x > blobVector.at(pixelValue-1).mostRightPixel) {blobVector.at(pixelValue-1).mostRightPixel = x;}

                /// Calculate perimeter.
                borderingSides = neighbourCount(img, x, y, 0, FOUR) - 4;
                if(borderingSides == 1) {blobVector.at(pixelValue-1).perimeter += 1;}
                else if(borderingSides == 2) {blobVector.at(pixelValue-1).perimeter += sqrt(2);}
                else if(borderingSides == 3) {blobVector.at(pixelValue-1).perimeter += sqrt(5);}

                /// Calculate sum of X and Y values of a blob. This is used to calculate centroid.
                blobVector.at(pixelValue-1).sumX += pow(x, 1);
                blobVector.at(pixelValue-1).sumY += pow(y, 1);
            }
        }
    }

    ///Calculate height, width, centroid and form factor.
    for(i=0; i<blobcount; i++){
        blobVector.at(i).height = blobVector.at(i).mostLowerPixel - blobVector.at(i).mostUpperPixel;
        blobVector.at(i).width = blobVector.at(i).mostRightPixel - blobVector.at(i).mostLeftPixel;
        blobVector.at(i).xCentroid = (float)blobVector.at(i).sumX / blobVector.at(i).nof_pixels;
        blobVector.at(i).yCentroid = (float)blobVector.at(i).sumY / blobVector.at(i).nof_pixels;
        blobVector.at(i).formFactor = pow(blobVector.at(i).perimeter, 2)/blobVector.at(i).nof_pixels;
    }

    /// Calculate normalized central moment for each blob.
    normalizedCentralMoments(img, blobcount);
}

/**
 * @brief Classifier::normalizedCentralMoments
 * @param img
 * @param blobcount
 */
void Classifier::normalizedCentralMoments(cv::Mat *img, const uint8_t blobcount)
{
    register uint16_t y = 0;
    register uint16_t x = 0;
    register uint8_t i = 0;
    register uint8_t pixelValue = 0;
    register const uint8_t Y = 2;

    /// Calculate central moments.
    for(y=0; y<img->rows; y++){
        for(x=0; x<img->cols; x++){
            pixelValue = img->at<uint8_t>(y, x);
            if(pixelValue != 0){
                blobVector.at(pixelValue-1).N20 += pow((x - blobVector.at(pixelValue-1).xCentroid), 2) * pow((y - blobVector.at(pixelValue-1).yCentroid), 0);
                blobVector.at(pixelValue-1).N20 += pow((x - blobVector.at(pixelValue-1).xCentroid), 0) * pow((y - blobVector.at(pixelValue-1).yCentroid), 2);
                blobVector.at(pixelValue-1).U00 += 1;
            }
        }
    }

    for(i=0; i<blobcount; i++){
        blobVector.at(i).N20 = blobVector.at(i).N20 / pow(blobVector.at(i).U00, Y);
        blobVector.at(i).N02 = blobVector.at(i).N20 / pow(blobVector.at(i).U00, Y);
        blobVector.at(i).invarianceMoment1 = blobVector.at(i).N20 + blobVector.at(i).N02;    
    }    
}

/**
 * @brief DIP::setSelected
 * @param src
 * @param dst
 * @param selected
 * @param value
 */
void Classifier::setSelected(cv::Mat *src, cv::Mat *dst, uint8_t selected, uint8_t value)
{
    register uint8_t *s = (uint8_t *)src->data;
    register uint8_t *d = (uint8_t *)dst->data;
    register uint32_t i = (src->rows * src->cols);

    /// Set selected pixels to value.
    while(i > 0){
        (*s == selected) ? *d = value : *d = *s;
        s++;
        d++;
        i--;
    }
}

/**
 * @brief Classifier::neighbourCount
 * @param img
 * @param x
 * @param y
 * @param value
 * @param connected
 * @return
 */
uint8_t Classifier::neighbourCount(cv::Mat *img, uint16_t x, uint16_t y, uint8_t value, uint8_t connected)
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
    for(yMask=0; yMask<n; yMask++){
        for(xMask=0; xMask<n; xMask++){
            if((y+yMask-1) < 0 || (y+yMask-1) >= img->rows){
//                cerr << "Out of image." << endl;
            }
            else if((x+xMask-1) < 0 || (x+xMask-1) >= img->cols){
//                cerr << "Out of image." << endl;
            }
            else if((yMask == 1) && (xMask == 1)){
//                cerr << "Anchor pixel." << endl;
            }
            else{
                pixelValue = (mask[yMask][xMask] * img->at<uint8_t>((y+yMask-1), (x+xMask-1)));
                if(selectedValue == pixelValue){
                    neighbourCount++;
                }
            }
        }
    }
    return neighbourCount;
}


/**
 * @brief Classifier::neighboursLowest
 * @param img
 * @param x
 * @param y
 * @param connected
 * @return
 */
uint8_t Classifier::neighboursLowest(cv::Mat *img, uint16_t x, uint16_t y, uint8_t connected)
{
    register uint8_t yMask = 0;
    register uint8_t xMask = 0;
    register uint8_t pixelValue = 0;
    register uint8_t lowestPixel = 255;
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
    for(yMask=0; yMask<n; yMask++){
        for(xMask=0; xMask<n; xMask++){
            if((y+yMask-1) < 0 || (y+yMask-1) >= img->rows){
//                QDEBUG("Out of image.");
            }
            else if((x+xMask-1) < 0 || (x+xMask-1) >= img->cols){
//                QDEBUG("Out of image.");
            }
            else if((yMask == 1) && (xMask == 1)){
//                QDEBUG("Anchor pixel.");
            }
            else{
                pixelValue = (mask[yMask][xMask] * img->at<uint8_t>((y+yMask-1), (x+xMask-1)));
                if((pixelValue < lowestPixel) && (pixelValue != 0)){
                    lowestPixel = (mask[yMask][xMask] * pixelValue);
                }
            }
        }
    }
    return lowestPixel;
}

/**
 * @brief Classifier::setSourceImage
 * @param im
 */
void Classifier::setSourceImage(cv::Mat *im)
{
    src = im;

    /// Create empty destination image.
    dst = cv::Mat::zeros(src->size(), CV_8UC1);
}

/**
 * @brief Classifier::getCarVector
 * @return
 */
vector<Car> * Classifier::getCarVector()
{
    return &carVector;
}

