#include "imageprocessor.h"

ImageProcessor::ImageProcessor(ClosestNeighbourScale *closest_neighbour_scale,
                               BilinearInterpolationScale *bilinear_scale,
                               BiQubicInterpolationScale *biqubic_scale,
                               Rotator *rotator) :
      _closest_neighbour_scale(closest_neighbour_scale)
    , _bilinear_scale(bilinear_scale)
    , _biqubic_scale(biqubic_scale)
    , _rotator(rotator){
}

void ImageProcessor::load_image(const QString& filepath) {
    _filepath = filepath;
    _source_image = _processed_image = QImage(_filepath);
    _source_image = _scaled_image = scale_image(_source_image, _preview_label_size, _preview_label_size);
}

void ImageProcessor::save_image() {
    QString fileName = QFileDialog::getSaveFileName();
    if (fileName.isEmpty())
        return;
    _processed_image.save(fileName);
}
