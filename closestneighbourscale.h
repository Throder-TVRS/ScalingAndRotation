#ifndef CLOSESTNEIGHBOURSCALE_H
#define CLOSESTNEIGHBOURSCALE_H
#include "utility.h"

class ClosestNeighbourScale {
public:
    ClosestNeighbourScale();

    double scale = 1;

    QImage default_image;

    QImage scale_image(const QImage& image, uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2);
};

#endif // CLOSESTNEIGHBOURSCALE_H
