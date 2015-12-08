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
	
    threshold(src, &dst, 230, 255);
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
        s++;
        i--;
    }

    /// Calculates the stretch factor.
    stretchFactor = ((float)(top-bottom)/(highestPixel-lowestPixel));

    /// Creating a look-up-table
    i = 0;
    while(i < 256){
        lut[i] = (uint8_t)((i-lowestPixel)*stretchFactor+0.5f+bottom);
        i++;
    }

    /// Initialize counters.
    s = (uint8_t *)src->data;
    d = (uint8_t *)dst->data;
    i = (src->rows * src->cols);

    /// Calulate new image with parameters and stretch factor.
    while(i > 0){
        *d = lut[*s];
        d++;
        s++;
        i--;
    }
}

/**
 * @brief DIP::threshold
 * @param src, pointer to source image.
 * @param dst, pointer to destination image.
 * @param low, low threshold value.
 * @param high, high threshold value.
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
        if((i > low) && (i < high)) {lut[i] = 255;}
        else {lut[i] = 0;}
        i++;
    }

    /// Initialize counter.
    i = (src->rows * src->cols);

    /// Reads pixel values, if value if between high and low make it 1, else 0.
    while(i > 0){
        *d = lut[*s];
        d++;
        s++;
        i--;
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
        histogram++;
        i++;
    }

    histogram = hist;
    i = (im->rows * im->cols);

    /// Create histogram
    while(i > 0){
        *(histogram + *s) += 1;
        *sumHistogram += *s;
        s++;
        i--;
    }
}

/**
 * @brief DIP::thresholdIsoData
 * @param src, pointer to source image.
 * @param dst, pointer to destination image.
 * @param brightness
 */
void DIP::thresholdIsoData(cv::Mat *src, cv::Mat *dst, uint8_t brightness)
{

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
        i++;
    }

    /// Initialize counter.
    i = (src->rows * src->cols);

    /// Invert each pixel.
    while(i > 0){
        *d = lut[*s];
        s++;
        d++;
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
        s++;
        d++;
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
        s++;
        d++;
        i--;
    }
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

