#include "utility.h"

uint64_t uint64_round(uint64_t numenator, uint64_t denumenator, uint64_t lim) {
    if(numenator % denumenator > lim)
        numenator += denumenator - 1;
    return numenator /= denumenator;
}

QImage scale_image(const QImage& image, int width, int height) {
    QImage scaled_image;
    int image_width = image.width();
    int image_height = image.height();
    if(image_height >= image_width && image_height > height)
        scaled_image = image.scaledToHeight(height);
    if(image_height < image_width && image_width > width)
        scaled_image = image.scaledToWidth(width);
    if(scaled_image.isNull())
        scaled_image = image;
    return scaled_image;
}

QImage expand_image(const QImage& image, int filter_size) {
    QImage new_image = QImage(image.width() + filter_size - 1, image.height() + filter_size - 1, QImage::Format_RGB32);
    QColor color;
    uint64_t offset = filter_size / 2;
    for(uint64_t x = 0; x < uint64_t(image.width()); x++)
        for(uint64_t y = 0; y < uint64_t(image.height()); y++) {
            color = image.pixelColor(x, y);
            new_image.setPixelColor(x + offset, y + offset, color);
        }
    for(uint64_t x = offset; x < uint64_t(new_image.width() - offset); x++) {
        color = new_image.pixelColor(x, offset).red();
        for(uint64_t i = 0; i < offset; i++)
            new_image.setPixelColor(x, i, color);
        color = new_image.pixelColor(x, new_image.height() - 1 - offset).red();
        for(uint64_t i = new_image.height() - offset; i < uint64_t(new_image.height()); i++)
            new_image.setPixelColor(x, i, color);
    }
    for(uint64_t y = 0; y < uint64_t(new_image.height()); y++) {
        color = new_image.pixelColor(offset, y).red();
        for(uint64_t i = 0; i < offset; i++)
            new_image.setPixelColor(i, y, color);
        color = new_image.pixelColor(new_image.width() - 1 - offset, y).red();
        for(uint64_t i = new_image.width() - offset; i < uint64_t(new_image.width()); i++)
            new_image.setPixelColor(i, y, color);
    }
    return new_image;
}
