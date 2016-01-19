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
#define RECTANGLE_NCM 0.171641
#define TRIANGLE_NCM 0.205000
#define MOON_NCM 0.358187
#define PLUS_NCM 0.275
#define MINUS_NCM 0.300417
#define FINISH_NCM 0.22500
#define NCM_TOLERANCE 0.2
#define NCM_FINISHTOLERANCE 0.2
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
#define FINISH_NCM_MIN FINISH_NCM - FINISH_NCM * NCM_FINISHTOLERANCE
#define FINISH_NCM_MAX FINISH_NCM + FINISH_NCM * NCM_FINISHTOLERANCE

#define MIN_AREA_OF_BLOB 20
#define MAX_AREA_OF_BLOB 100
#define MIN_AREA_OF_FINISH 50
#define MAX_AREA_OF_FINISH 250

///
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

///
typedef struct finish_t
{
    uint8_t blobLabel = 0;
    float invarianceMoment1 = 0;

}finish_t;


/**
 * @brief The Classifier class
 */
class Classifier
{
public:
    Classifier();
    ~Classifier();
    void classifyCars();
    void classifyFinish();
    void setSourceImage(cv::Mat * im);
    cv::Mat *getLabeldImage();
    std::vector<Car> *getCarVector();
    finish_t *getFinish();

private:
    uint32_t labelBlobs(cv::Mat * src, cv::Mat * dst, uint8_t connected);
    void blobAnalyse(cv::Mat * im, const uint8_t nrBlobs);
    void blobAnalyseFinish(cv::Mat * im, const uint8_t nrBlobs);
    void normalizedCentralMoments(cv::Mat *img, const uint8_t blobcount);
    void normalizedCentralMomentsFinish(cv::Mat *img, const uint8_t blobcount);
    void threshold(cv::Mat *src, cv::Mat *dst, uint8_t low, uint8_t high);
    void setSelected(cv::Mat *src, cv::Mat *dst, uint8_t selected, uint8_t value);
    uint8_t neighbourCount(cv::Mat *img, uint16_t x, uint16_t y, uint8_t value, uint8_t connected);
    uint8_t neighboursLowest(cv::Mat *img, uint16_t x, uint16_t y, uint8_t connected);

    cv::Mat *src;
    cv::Mat dst;

    std::vector<blobinfo_t> blobVector;
    finish_t finish;
    std::vector<Car> carVector;

    uint8_t maskFourConnected[3][3] = {{0, 1, 0},
                                       {1, 0, 1},
                                       {0, 1, 0}};
    uint8_t maskEightConnected[3][3] = {{1, 1, 1},
                                        {1, 0, 1},
                                        {1, 1, 1}};
};

#endif // CLASSIFIER_H