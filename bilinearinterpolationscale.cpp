#include "bilinearinterpolationscale.h"

BilinearInterpolationScale::BilinearInterpolationScale() {

}

QImage BilinearInterpolationScale::scale_image(const QImage &image, uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2) {
    //Получение координат X1, Y1, X2, Y2 границ области на исходном большом изображении по координатам области с превьюшки
    if(x2 < x1)
        std::swap(x1, x2);
    if(y2 < y1)
        std::swap(y1, y2);
    x1 = uint64_t(x1 * double(default_image.width()) / image.width() + 0.5);
    x2 = uint64_t(x2 * double(default_image.width()) / image.width() + 0.5);
    y1 = uint64_t(y1 * double(default_image.height()) / image.height() + 0.5);
    y2 = uint64_t(y2 * double(default_image.height()) / image.height() + 0.5);

    //Построение увеличенного изображеия области
    QImage new_image = QImage(scale * (x2 - x1 + 1), scale * (y2 - y1 + 1), QImage::Format_RGB32);
    double t, s, _x, _y;
    QColor color;
    uint64_t u, v;
    for(uint64_t x = 0; x < new_image.width(); x++) {
        for(uint64_t y = 0; y < new_image.height(); y++) {
            _x = x1 + double(x) / scale;
            _y = y1 + double(y) / scale;
            u = uint64_t(_x);
            v = uint64_t(_y);
            s = _x - u;
            t = _y - v;
            color.setRed((1 - s) * (1 - t) * default_image.pixelColor(u, v).red() +
                         s * (1 - t) * default_image.pixelColor(u + 1, v).red() +
                         (1 - s) * t * default_image.pixelColor(u, v + 1).red() +
                         s * t * default_image.pixelColor(u + 1, v + 1).red());
            color.setBlue((1 - s) * (1 - t) * default_image.pixelColor(u, v).blue() +
                         s * (1 - t) * default_image.pixelColor(u + 1, v).blue() +
                         (1 - s) * t * default_image.pixelColor(u, v + 1).blue() +
                         s * t * default_image.pixelColor(u + 1, v + 1).blue());
            color.setGreen((1 - s) * (1 - t) * default_image.pixelColor(u, v).green() +
                         s * (1 - t) * default_image.pixelColor(u + 1, v).green() +
                         (1 - s) * t * default_image.pixelColor(u, v + 1).green() +
                         s * t * default_image.pixelColor(u + 1, v + 1).green());
            new_image.setPixelColor(x, y, color);
        }
    }
    default_image = new_image;
    return new_image;
}
