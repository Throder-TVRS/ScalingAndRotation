#include "mainwindow.h"
#include "imageprocessor.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ClosestNeighbourScale closest_neighbour_scale;
    BilinearInterpolationScale bilinear_scale;
    BiQubicInterpolationScale biqubic_scale;
    Rotator rotator;
    ImageProcessor processor(&closest_neighbour_scale,
                             &bilinear_scale,
                             &biqubic_scale,
                             &rotator);
    MainWindow w(nullptr ,&processor);
    w.setFixedSize(w.size());
    w.setWindowTitle("Image processor");
    w.show();
    return a.exec();
}
