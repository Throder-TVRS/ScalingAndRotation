#ifndef ROTATOR_H
#define ROTATOR_H
#include "utility.h"

class Rotator {
public:
    Rotator();

    double angle = 0;
    int64_t center_x = -1, center_y = -1;
    int64_t x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    bool center_choose_status = false;
    bool use_status = false;

    QImage default_image;
    QImage template_image;

    QImage rotate_region(const QImage& image);
    QImage rotate();
};

#endif // ROTATOR_H
