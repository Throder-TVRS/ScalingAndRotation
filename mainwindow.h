#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QPushButton>
#include <QPainter>
#include <QCursor>
#include "imageprocessor.h"
class ImageProcessor;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    ImageProcessor *_processor;

public:
    MainWindow(QWidget *parent = nullptr, ImageProcessor *processor = nullptr);
    ~MainWindow();

    void update_image();
    void update_image(const QImage& image);

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void on_load_image_button_clicked();
    void on_reset_button_clicked();
    void on_save_image_button_clicked();
    void on_closest_neighbor_use_button_clicked();
    void on_unlock_cursor_checkbox_stateChanged(int arg1);
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_bilinear_use_button_clicked();
    void on_bilinear_spinbox_valueChanged(double arg1);
    void on_biqubic_use_button_clicked();
    void on_biqubic_spinbox_valueChanged(double arg1);
    void on_rotator_use_button_clicked();
    void on_rotator_center_button_clicked();
    void on_rotator_angle_slider_valueChanged(int value);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
