#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "car.h"

#define FOUR 4
#define EIGHT 8

#define CIRCLE_NCM 0.160013
#define RECTANGLE_NCM 0.166969
#define TRIANGLE_NCM 0.193509
#define MOON_NCM 0.206975
#define PLUS_NCM 0.260259
#define MINUS_NCM 0.415905
#define NCM_TOLERANCE 0.025
#define CIRCLE_NCM_MIN CIRCLE_NCM - CIRCLE_NCM * NCM_TOLERANCE
#define CIRCLE_NCM_MAX CIRCLE_NCM + CIRCLE_NCM * NCM_TOLERANCE
#define RECTANGLE_NCM_MIN RECTANGLE_NCM - RECTANGLE_NCM * NCM_TOLERANCE
#define RECTANGLE_NCM_MAX RECTANGLE_NCM + RECTANGLE_NCM * NCM_TOLERANCE
#define TRIANGLE_NCM_MIN TRIANGLE_NCM - TRIANGLE_NCM * NCM_TOLERANCE
#define TRIANGLE_NCM_MAX TRIANGLE_NCM + TRIANGLE_NCM * NCM_TOLERANCE
#define MOON_NCM_MIN MOON_NCM - MOON_NCM * NCM_TOLERANCE
#define MOON_NCM_MAX MOON_NCM + MOON_NCM * NCM_TOLERANCE
#define PLUS_NCM_MIN PLUS_NCM - PLUS_NCM * NCM_TOLERANCE
#define PLUS_NCM_MAX PLUS_NCM + PLUS_NCM * NCM_TOLERANCE
#define MINUS_NCM_MIN MINUS_NCM - MINUS_NCM * NCM_TOLERANCE
#define MINUS_NCM_MAX MINUS_NCM + MINUS_NCM * NCM_TOLERANCE

typedef struct blobinfo_t
{
    uint16_t mostUpperPixel = 0;
    uint16_t mostLowerPixel = 0;
    uint16_t mostLeftPixel = 0;
    uint16_t mostRightPixel = 0;
    uint32_t sumX = 0;
    uint32_t sumY = 0;
    uint16_t height = 0;
    uint16_t width = 0;
    uint16_t nof_pixels = 0;
    uint16_t xCentroid = 0;
    uint16_t yCentroid = 0;
    float perimeter = 0;
    float formFactor = 0;
    uint32_t U00 = 0;
    float N20 = 0;
    float N02 = 0;
    float invarianceMoment1 = 0;

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
    void normalizedCentralMoments(cv::Mat *img, const uint8_t blobcount);
    void setSelected(cv::Mat *src, cv::Mat *dst, uint8_t selected, uint8_t value);
    uint8_t neighbourCount(cv::Mat *img, uint16_t x, uint16_t y, uint8_t value, uint8_t connected);
    uint8_t neighboursLowest(cv::Mat *img, uint16_t x, uint16_t y, uint8_t connected);

    cv::Mat * src;
    cv::Mat dst;

    std::vector<blobinfo_t> blobVector;
    std::vector<Car> carVector;
};

#endif // CLASSIFIER_H