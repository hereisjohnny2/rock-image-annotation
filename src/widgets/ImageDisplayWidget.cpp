//
// Created by joao on 19/03/2022.
//

#include <QMouseEvent>
#include "ImageDisplayWidget.h"

ImageDisplayWidget::ImageDisplayWidget() {
    setBackgroundRole(QPalette::Base);
    setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
    setScaledContents(true);
}

void ImageDisplayWidget::mouseMoveEvent(QMouseEvent *mouseEvent) {
    QPoint currentPoint = mouseEvent->pos();

    QRgb color = QColor(image.pixel(currentPoint)).rgb();
}

void ImageDisplayWidget::setImage(const QImage &newImage) {
    image = newImage;
    setPixmap(QPixmap::fromImage(image));
    adjustSize();
}

const QImage &ImageDisplayWidget::getImage() const {
    return image;
}

