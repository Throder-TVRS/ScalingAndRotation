#include "closestneighbourscale.h"

ClosestNeighbourScale::ClosestNeighbourScale() {

}

QImage ClosestNeighbourScale::scale_image(const QImage &image, uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2) {
    //Получение координат X1, Y1, X2, Y2 границ области на исходном большом изображении по координатам области с превьюшки
    if(x2 < x1)
        std::swap(x1, x2);
    if(y2 < y1)
        std::swap(y1, y2);
    double X1, Y1;
    x1 = uint64_t(x1 * double(default_image.width()) / image.width() + 0.5);
    x2 = uint64_t(x2 * double(default_image.width()) / image.width() + 0.5);
    y1 = uint64_t(y1 * double(default_image.height()) / image.height() + 0.5);
    y2 = uint64_t(y2 * double(default_image.height()) / image.height() + 0.5);

    //Построение увеличенного изображеия области
    QImage new_image = QImage(scale * (x2 - x1 + 1), scale * (y2 - y1 + 1), QImage::Format_RGB32);
    for(uint64_t x = 0; x < new_image.width(); x++) {
        for(uint64_t y = 0; y < new_image.height(); y++) {
            X1 = uint64_t(x1 + double(x) / scale + 0.5);
            Y1 = uint64_t(y1 + double(y) / scale + 0.5);
            new_image.setPixelColor(x, y, default_image.pixelColor(X1, Y1));
        }
    }
    default_image = new_image;
    return new_image;
}
