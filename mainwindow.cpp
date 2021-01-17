#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QColorDialog>
#include <QMouseEvent>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent, ImageProcessor *processor)
    : QMainWindow(parent)
    , _processor(processor)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::update_image() {
    if(_processor->_filepath.isEmpty())
        return;
    _processor->_scaled_image = scale_image(_processor->_processed_image, _processor->_preview_label_size, _processor->_preview_label_size);
    ui->preview_label->setPixmap(QPixmap::fromImage(_processor->_scaled_image));
    _processor->region_setted = false;
    _processor->_closest_neighbour_scale->default_image = _processor->_processed_image;
    _processor->_bilinear_scale->default_image = _processor->_processed_image;
    _processor->_biqubic_scale->default_image = _processor->_processed_image;
    _processor->_rotator->default_image = _processor->_processed_image;
}

void MainWindow::update_image(const QImage& image) {
    if(_processor->_filepath.isEmpty())
        return;
    ui->preview_label->setPixmap(QPixmap::fromImage(scale_image(image, _processor->_preview_label_size, _processor->_preview_label_size)));
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if(!_processor->cursor_unlocked || _processor->_processed_image.isNull())
        return;
    if(event->x() < 0 || event->x() >= _processor->_scaled_image.width() ||
       event->y() < 0 || event->y() >= _processor->_scaled_image.height())
            return;
    _processor->_x2 = event->x();
    _processor->_y2 = event->y();
    ui->x2_lcd->display(event->x());
    ui->y2_lcd->display(event->y());

    uint64_t x1 = _processor->_x1;
    uint64_t x2 = _processor->_x2;
    uint64_t y1 = _processor->_y1;
    uint64_t y2 = _processor->_y2;

    QImage buf = _processor->_scaled_image;
    if(x2 < x1)
        std::swap(x1, x2);
    if(y2 < y1)
        std::swap(y1, y2);
    for(uint64_t x = x1; x <= x2; x++) {
        for(uint64_t y = y1; y <= y1 + 1; y++) {
            buf.setPixelColor(x, y, qRgb(255, 0, 0));
        }
        for(uint64_t y = y2 - 1; y <= y2; y++) {
            buf.setPixelColor(x, y, qRgb(255, 0, 0));
        }
    }
    for(uint64_t y = y1; y <= y2; y++) {
        for(uint64_t x = x1; x <= x1 + 1; x++) {
            buf.setPixelColor(x, y, qRgb(255, 0, 0));
        }
        for(uint64_t x = x2 - 1; x <= x2; x++) {
            buf.setPixelColor(x, y, qRgb(255, 0, 0));
        }
    }
    if(_processor->_rotator->center_x > -1) {
        buf.setPixelColor(_processor->_rotator->center_x, _processor->_rotator->center_y, qRgb(255, 0, 0));
        if(_processor->_rotator->center_x > 0)
            buf.setPixelColor(_processor->_rotator->center_x - 1, _processor->_rotator->center_y, qRgb(255, 0, 0));
        if(_processor->_rotator->center_x < buf.width() - 1)
            buf.setPixelColor(_processor->_rotator->center_x + 1, _processor->_rotator->center_y, qRgb(255, 0, 0));
        if(_processor->_rotator->center_y > 0)
            buf.setPixelColor(_processor->_rotator->center_x, _processor->_rotator->center_y - 1, qRgb(255, 0, 0));
        if(_processor->_rotator->center_y < buf.height() - 1)
            buf.setPixelColor(_processor->_rotator->center_x, _processor->_rotator->center_y + 1, qRgb(255, 0, 0));
    }
    update_image(buf);
    _processor->region_setted = true;
    QWidget::mouseMoveEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if(!_processor->cursor_unlocked || _processor->_processed_image.isNull())
        return;
    if(event->x() < 0 || event->x() >= _processor->_scaled_image.width() ||
       event->y() < 0 || event->y() >= _processor->_scaled_image.height())
            return;
    if(_processor->_rotator->center_choose_status) {
        _processor->_rotator->center_x = event->x();
        _processor->_rotator->center_y = event->y();
        _processor->_rotator->center_choose_status = false;
        ui->rotator_center_button->setFlat(false);
        ui->rotator_center_x->display(event->x());
        ui->rotator_center_y->display(event->y());
        QImage image = ui->preview_label->pixmap()->toImage();
        image.setPixelColor(event->x(), event->y(), qRgb(255, 0, 0));
        if(event->x() > 0)
            image.setPixelColor(event->x() - 1, event->y(), qRgb(255, 0, 0));
        if(event->x() < image.width() - 1)
            image.setPixelColor(event->x() + 1, event->y(), qRgb(255, 0, 0));
        if(event->y() > 0)
            image.setPixelColor(event->x(), event->y() - 1, qRgb(255, 0, 0));
        if(event->y() < image.height() - 1)
            image.setPixelColor(event->x(), event->y() + 1, qRgb(255, 0, 0));
        update_image(image);
        QWidget::mousePressEvent(event);
        return;
    }
    _processor->_x1 = event->x();
    _processor->_y1 = event->y();
    update_image();
    ui->x1_lcd->display(event->x());
    ui->y1_lcd->display(event->y());
    _processor->region_setted = false;
    QWidget::mousePressEvent(event);
}

void MainWindow::on_load_image_button_clicked() {
    QString new_filepath = QFileDialog::getOpenFileName(0, "File selection", "", "*.jpg *.png *.bmp");
    if(new_filepath.isEmpty() || _processor->_filepath == new_filepath)
        return;
    _processor->load_image(new_filepath);
    _processor->_x1 = _processor->_y1 = 0;
    _processor->_x2 = scale_image(_processor->_processed_image, _processor->_preview_label_size, _processor->_preview_label_size).width() - 1;
    _processor->_y2 = scale_image(_processor->_processed_image, _processor->_preview_label_size, _processor->_preview_label_size).height() - 1;
    _processor->region_setted = false;
    update_image();
    on_reset_button_clicked();
}

void MainWindow::on_reset_button_clicked() {
    _processor->_processed_image = _processor->_source_image;
    _processor->_rotator->x1 = 0;
    _processor->_rotator->x2 = 0;
    _processor->_rotator->y1 = 0;
    _processor->_rotator->y2 = 0;
    _processor->_rotator->center_x = -1;
    _processor->_rotator->center_y = -1;
    ui->rotator_center_x->display(-1);
    ui->rotator_center_y->display(-1);
    _processor->_rotator->center_choose_status = false;
    _processor->_rotator->use_status = false;
    update_image();
}

void MainWindow::on_save_image_button_clicked() {
    _processor->save_image();
}

void MainWindow::on_closest_neighbor_use_button_clicked() {
    ImageProcessor *p = _processor;
    if(!p->cursor_unlocked ||
       !p->region_setted)
        return;
    p->_processed_image = p->_closest_neighbour_scale->scale_image(p->_scaled_image, p->_x1, p->_y1, p->_x2, p->_y2);
    update_image();
}

void MainWindow::on_unlock_cursor_checkbox_stateChanged(int arg1) {
    if(arg1 == Qt::CheckState::Checked)
        _processor->cursor_unlocked = true;
    if(arg1 == Qt::CheckState::Unchecked)
        _processor->cursor_unlocked = false;
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1) {
    _processor->_closest_neighbour_scale->scale = arg1;
}

void MainWindow::on_bilinear_use_button_clicked() {
    ImageProcessor *p = _processor;
    if(!p->cursor_unlocked ||
       !p->region_setted)
        return;
    p->_processed_image = p->_bilinear_scale->scale_image(p->_scaled_image, p->_x1, p->_y1, p->_x2, p->_y2);
    update_image();
}

void MainWindow::on_bilinear_spinbox_valueChanged(double arg1) {
    _processor->_bilinear_scale->scale = arg1;
}

void MainWindow::on_biqubic_use_button_clicked() {
    ImageProcessor *p = _processor;
    if(!p->cursor_unlocked ||
       !p->region_setted)
        return;
    p->_processed_image = p->_biqubic_scale->scale_image(p->_scaled_image, p->_x1, p->_y1, p->_x2, p->_y2);
    update_image();
}

void MainWindow::on_biqubic_spinbox_valueChanged(double arg1) {
    _processor->_biqubic_scale->scale = arg1;
}

void MainWindow::on_rotator_use_button_clicked() {
    ImageProcessor *p = _processor;
    if(!p->cursor_unlocked ||
       !p->region_setted ||
       p->_rotator->center_choose_status ||
       p->_rotator->center_x == -1)
        return;
    p->_rotator->x1 = p->_x1;
    p->_rotator->x2 = p->_x2;
    p->_rotator->y1 = p->_y1;
    p->_rotator->y2 = p->_y2;
    update_image(p->_rotator->rotate_region(p->_rotator->default_image));
    p->_rotator->use_status = true;
}

void MainWindow::on_rotator_center_button_clicked() {
    if(_processor->_rotator->center_choose_status)
        return;
    _processor->_rotator->center_choose_status = true;
    ui->rotator_center_button->setFlat(true);
}

void MainWindow::on_rotator_angle_slider_valueChanged(int value) {
    _processor->_rotator->angle = 0.01745 * value;
    ui->rotator_angle_lcd->display(value);
    if(_processor->_rotator->center_x != -1 &&
       _processor->_rotator->use_status) {
        update_image(_processor->_rotator->rotate());
    }
}
