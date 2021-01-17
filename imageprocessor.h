#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <QColorDialog>

#include "utility.h"
#include "closestneighbourscale.h"
#include "bilinearinterpolationscale.h"
#include "biqubicinterpolationscale.h"
#include "rotator.h"

class ClosestNeighbourScale;
class BilinearInterpolationScale;
class BiQubicInterpolationScale;
class Rotator;

class ImageProcessor {
public:
    ImageProcessor(ClosestNeighbourScale *closest_neighbour_scale,
                   BilinearInterpolationScale *bilinear_scale,
                   BiQubicInterpolationScale *biqubic_scale,
                   Rotator *rotator);

    QString _filepath;

    QImage _source_image;
    QImage _processed_image;
    QImage _scaled_image;

    bool cursor_unlocked = false;
    bool region_setted = false;
    uint64_t _x1, _x2, _y1, _y2;
    uint64_t _scaled_width = 711, _scaled_height = 711;

    const uint64_t _preview_label_size = 711;

    ClosestNeighbourScale *_closest_neighbour_scale;
    BilinearInterpolationScale *_bilinear_scale;
    BiQubicInterpolationScale *_biqubic_scale;
    Rotator *_rotator;

    void load_image(const QString& filepath);
    void save_image();
};

#endif // IMAGEPROCESSOR_H
