#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "car.h"

#define FOUR 4
#define EIGHT 8

typedef struct blobinfo_t
{
    uint16_t mostUpperPixel = 0;
    uint16_t mostLowerPixel = 0;
    uint16_t mostLeftPixel = 0;
    uint16_t mostRightPixel = 0;
    uint16_t height = 0;
    uint16_t width = 0;
    uint16_t nof_pixels = 0;
    float perimeter = 0;
    float formFactor = 0;

}blobinfo_t;

/**
 * @brief The Classifier class
 */
class Classifier
{
public:
    Classifier();
    void classifyCars();
    void setSourceImage(cv::Mat * im);
    std::vector<Car> * getCarVector();

private:
    uint32_t labelBlobs(cv::Mat * src, cv::Mat * dst, uint8_t connected);
    void blobAnalyse(cv::Mat * im, const uint8_t nrBlobs);
    void setSelected(cv::Mat *src, cv::Mat *dst, uint8_t selected, uint8_t value);
    uint8_t neighbourCount(cv::Mat *img, uint16_t x, uint16_t y, uint8_t value, uint8_t connected);
    uint8_t neighboursLowest(cv::Mat *img, uint16_t x, uint16_t y, uint8_t connected);

    cv::Mat * src;
    cv::Mat dst;

    std::vector<blobinfo_t> blobVector;
    std::vector<Car> carVector;
};

#endif // CLASSIFIER_H