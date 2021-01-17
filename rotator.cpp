#include "rotator.h"

Rotator::Rotator() {

}

QImage Rotator::rotate_region(const QImage &image) {
    center_x = uint64_t(center_x * double(default_image.width()) / image.width() + 0.5);
    center_y = uint64_t(center_y * double(default_image.height()) / image.height() + 0.5);
    if(x2 < x1)
        std::swap(x1, x2);
    if(y2 < y1)
        std::swap(y1, y2);
    x1 = uint64_t(x1 * double(default_image.width()) / image.width() + 0.5);
    x2 = uint64_t(x2 * double(default_image.width()) / image.width() + 0.5);
    y1 = uint64_t(y1 * double(default_image.height()) / image.height() + 0.5);
    y2 = uint64_t(y2 * double(default_image.height()) / image.height() + 0.5);

    template_image = default_image;
    for(int x = x1; x <= x2; x++)
        for(int y = y1; y <= y2; y++)
            template_image.setPixelColor(x, y, qRgb(0, 0, 0));
    return rotate();
}

QImage Rotator::rotate() {
    QImage new_image = template_image;
    if(x2 < x1)
        std::swap(x1, x2);
    if(y2 < y1)
        std::swap(y1, y2);
    int buf_x, buf_y;
    for(int x = x1; x <= x2; x++) {
        for(int y = y1; y <= y2; y++) {
            buf_x = center_x + (x - center_x) * std::cos(angle) - (y - center_y) * std::sin(angle);
            buf_y = center_y + (x - center_x) * std::sin(angle) + (y - center_y) * std::cos(angle);
            if(buf_x >= 0 && buf_x < new_image.width() && buf_y >= 0 && buf_y < new_image.height())
                new_image.setPixelColor(buf_x, buf_y, default_image.pixelColor(x, y));
        }
    }
    return new_image;
}
