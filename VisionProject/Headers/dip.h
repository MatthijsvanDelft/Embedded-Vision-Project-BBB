#ifndef DIP_H
#define DIP_H

#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "testbench.h"

#define DARK 0
#define BRIGHT 1
#define FOUR 4
#define EIGHT 8

/**
 * @brief The DIP class
 */
class DIP
{
public:
    DIP();
    void visionSet1();
    void visionSet2();
    void visionSet3();
    void setSourceImage(cv::Mat * im);
    cv::Mat * getEnhancedImage();

private:
    void contrastStretchFast(cv::Mat *src, cv::Mat *dst);    
    void histogram(cv::Mat *im, uint16_t *hist, uint32_t sum);
    void thresholdIsoData(cv::Mat *src, cv::Mat *dst, uint8_t brightness);
    void threshold(cv::Mat *src, cv::Mat *dst, uint8_t low, uint8_t high);
    void invert(cv::Mat *src, cv::Mat *dst);
    void setSelected(cv::Mat *src, cv::Mat *dst, uint8_t selected, uint8_t value);
    void multiply(cv::Mat *src, cv::Mat *dst);

    void removeBorderBlobs(cv::Mat *src, cv::Mat *dst, uint8_t connected);//Binary operator
    void fillHoles(cv::Mat *src, cv::Mat *dst, uint8_t connected);//Binary operator
    void binaryEdgeDetect(cv::Mat *src, cv::Mat *dst, uint8_t connected);//Binary operator

    uint8_t neighbourCount(cv::Mat *img, uint16_t x, uint16_t y, uint8_t value, uint8_t connected);
    uint8_t neighboursEqualOrHigher(cv::Mat *img, uint16_t x, uint16_t y, uint8_t value, uint8_t connected);

    cv::Mat *src;
    cv::Mat dst;
    Testbench testbench;
};

#endif // DIP_H
