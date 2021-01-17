#include "biqubicinterpolationscale.h"

BiQubicInterpolationScale::BiQubicInterpolationScale() {

}

double BiQubicInterpolationScale::get_intense(double x1, double x2, double x3, double x4, double y1, double y2, double y3, double y4, double target) {
    double a = x1*x1*x1, b = x1*x1;
    double k11 = x2*x2*x2 - a,
           k12 = x2*x2 - b,
           k13 = x2 - x1,
           k21 = x3*x3*x3 - a,
           k22 = x3*x3 - b,
           k23 = x3 - x1,
           k31 = x4*x4*x4 - a,
           k32 = x4*x4 - b,
           k33 = x4 - x1;
    double f1 = y2 - y1,
           f2 = y3 - y1,
           f3 = y4 - y1;
    double det =  k11*k22*k33 + k12*k23*k31 + k13*k32*k21 -
                  k13*k22*k31 - k11*k23*k32 - k33*k12*k21;
    double det1 =  f1*k22*k33 + k12*k23*f3  + k13*k32*f2  -
                  k13*k22*f3  -  f1*k23*k32 - k33*k12*f2;
    double det2 = k11*f2*k33  + f1*k23*k31  + k13*f3*k21  -
                  k13*f2*k31  - k11*k23*f3  - k33*f1*k21;
    double det3 = k11*k22*f3  + k12*f2*k31  + f1*k32*k21  -
                  f1*k22*k31  - k11*f2*k32  - f3*k12*k21;
    double A = det1 / det,
           B = det2 / det,
           C = det3 / det,
           D = y1 - A*a-B*b-C*x1;
    return std::max(std::min(A * target * target * target + B * target * target + C * target + D + 0.5, 255.0), 0.0);
}

QImage BiQubicInterpolationScale::scale_image(const QImage &image, uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2) {
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
    QImage _image = expand_image(default_image, 2);
    QImage new_image = QImage(scale * (x2 - x1 + 1), scale * (y2 - y1 + 1), QImage::Format_RGB32);
    double _x, _y, c1, c2, c3, c4;
    QColor color;
    uint64_t u, v;
    for(uint64_t x = 0; x < new_image.width(); x++) {
        for(uint64_t y = 0; y < new_image.height(); y++) {
            _x = x1 + double(x) / scale;
            _y = y1 + double(y) / scale;
            u = uint64_t(_x);
            v = uint64_t(_y);
            //RED
            c1 = get_intense(v - 1, v, v + 1, v + 2,
                             _image.pixelColor(u - 1, v - 1).red(),
                             _image.pixelColor(u - 1, v).red(),
                             _image.pixelColor(u - 1, v + 1).red(),
                             _image.pixelColor(u - 1, v + 2).red(),
                             _y);
            c2 = get_intense(v - 1, v, v + 1, v + 2,
                             _image.pixelColor(u, v - 1).red(),
                             _image.pixelColor(u, v).red(),
                             _image.pixelColor(u, v + 1).red(),
                             _image.pixelColor(u, v + 2).red(),
                             _y);
            c3 = get_intense(v - 1, v, v + 1, v + 2,
                             _image.pixelColor(u + 1, v - 1).red(),
                             _image.pixelColor(u + 1, v).red(),
                             _image.pixelColor(u + 1, v + 1).red(),
                             _image.pixelColor(u + 1, v + 2).red(),
                             _y);
            c4 = get_intense(v - 1, v, v + 1, v + 2,
                             _image.pixelColor(u + 2, v - 1).red(),
                             _image.pixelColor(u + 2, v).red(),
                             _image.pixelColor(u + 2, v + 1).red(),
                             _image.pixelColor(u + 2, v + 2).red(),
                             _y);
            color.setRed(uint64_t(get_intense(u-1, u, u + 1, u + 2,
                                     c1, c2, c3, c4, _x)));
            //GREEN
            c1 = get_intense(v - 1, v, v + 1, v + 2,
                             _image.pixelColor(u - 1, v - 1).green(),
                             _image.pixelColor(u - 1, v).green(),
                             _image.pixelColor(u - 1, v + 1).green(),
                             _image.pixelColor(u - 1, v + 2).green(),
                             _y);
            c2 = get_intense(v - 1, v, v + 1, v + 2,
                             _image.pixelColor(u, v - 1).green(),
                             _image.pixelColor(u, v).green(),
                             _image.pixelColor(u, v + 1).green(),
                             _image.pixelColor(u, v + 2).green(),
                             _y);
            c3 = get_intense(v - 1, v, v + 1, v + 2,
                             _image.pixelColor(u + 1, v - 1).green(),
                             _image.pixelColor(u + 1, v).green(),
                             _image.pixelColor(u + 1, v + 1).green(),
                             _image.pixelColor(u + 1, v + 2).green(),
                             _y);
            c4 = get_intense(v - 1, v, v + 1, v + 2,
                             _image.pixelColor(u + 2, v - 1).green(),
                             _image.pixelColor(u + 2, v).green(),
                             _image.pixelColor(u + 2, v + 1).green(),
                             _image.pixelColor(u + 2, v + 2).green(),
                             _y);
            color.setGreen(uint64_t(get_intense(u-1, u, u + 1, u + 2,
                                       c1, c2, c3, c4, _x)));

            //BLUE
            c1 = get_intense(v - 1, v, v + 1, v + 2,
                             _image.pixelColor(u - 1, v - 1).blue(),
                             _image.pixelColor(u - 1, v).blue(),
                             _image.pixelColor(u - 1, v + 1).blue(),
                             _image.pixelColor(u - 1, v + 2).blue(),
                             _y);
            c2 = get_intense(v - 1, v, v + 1, v + 2,
                             _image.pixelColor(u, v - 1).blue(),
                             _image.pixelColor(u, v).blue(),
                             _image.pixelColor(u, v + 1).blue(),
                             _image.pixelColor(u, v + 2).blue(),
                             _y);
            c3 = get_intense(v - 1, v, v + 1, v + 2,
                             _image.pixelColor(u + 1, v - 1).blue(),
                             _image.pixelColor(u + 1, v).blue(),
                             _image.pixelColor(u + 1, v + 1).blue(),
                             _image.pixelColor(u + 1, v + 2).blue(),
                             _y);
            c4 = get_intense(v - 1, v, v + 1, v + 2,
                             _image.pixelColor(u + 2, v - 1).blue(),
                             _image.pixelColor(u + 2, v).blue(),
                             _image.pixelColor(u + 2, v + 1).blue(),
                             _image.pixelColor(u + 2, v + 2).blue(),
                             _y);
            color.setBlue(uint64_t(get_intense(u-1, u, u + 1, u + 2,
                                      c1, c2, c3, c4, _x)));

            new_image.setPixelColor(x, y, color);
        }
    }
    default_image = new_image;
    return new_image;
}
