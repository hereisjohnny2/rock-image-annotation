#include <QMouseEvent>
#include "ImageDisplayWidget.h"

ImageDisplayWidget::ImageDisplayWidget() {
    setBackgroundRole(QPalette::Base);
    setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
    setScaledContents(true);
}

void ImageDisplayWidget::mouseMoveEvent(QMouseEvent *mouseEvent) {
    QPoint currentPoint = mouseEvent->pos();
    QRgb rgb = QColor(image.pixel(currentPoint)).rgb();
    pixelDataMap.insert(currentPoint, rgb);
}

void ImageDisplayWidget::setImage(const QImage &newImage) {
    image = newImage;
    setPixmap(QPixmap::fromImage(image));
    adjustSize();
}

const QImage &ImageDisplayWidget::getImage() const {
    return image;
}

const QHash<QPoint, QRgb> &ImageDisplayWidget::getPixelDataMap() const {
    return pixelDataMap;
}

void ImageDisplayWidget::clearPixelDataMap() {
    pixelDataMap = {};
}


