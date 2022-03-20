//
// Created by joao on 19/03/2022.
//

#include "ImageDisplayWidget.h"

ImageDisplayWidget::ImageDisplayWidget() {
    image = QImage();
    this->setPixmap(QPixmap::fromImage(image));
    this->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
    this->setScaledContents(true);
    this->setAlignment(Qt::AlignCenter);
}

void ImageDisplayWidget::openImage(const QString &filePath) {
    image = QImage(filePath);
    this->setPixmap(QPixmap::fromImage(image));
}

const PixelData &ImageDisplayWidget::getCollectedPixelData() const {
    return collectedPixelData;
}

void ImageDisplayWidget::resetCollectedData() {
    collectedPixelData = PixelData();
}

void ImageDisplayWidget::mouseMoveEvent(QMouseEvent *ev) {
    QLabel::mouseMoveEvent(ev);
}
