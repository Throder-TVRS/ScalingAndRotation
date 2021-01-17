#ifndef BIQUBICINTERPOLATIONSCALE_H
#define BIQUBICINTERPOLATIONSCALE_H
#include "utility.h"

class BiQubicInterpolationScale {
public:
    BiQubicInterpolationScale();

    double scale = 1;

    QImage default_image;

    double get_intense(double x1, double x2, double x3, double x4, double y1, double y2, double y3, double y4, double target);
    QImage scale_image(const QImage& image, uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2);
};

#endif // BIQUBICINTERPOLATIONSCALE_H
