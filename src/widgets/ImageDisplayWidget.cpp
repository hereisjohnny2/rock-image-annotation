#include <QMouseEvent>
#include <QPainter>
#include "ImageDisplayWidget.h"

ImageDisplayWidget::ImageDisplayWidget() {
    setBackgroundRole(QPalette::Base);
    setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
    setScaledContents(true);
}

void ImageDisplayWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
    }
}

void ImageDisplayWidget::mouseMoveEvent(QMouseEvent *event) {
    QPoint currentPoint = event->pos();
    QRgb rgb = QColor(image.pixel(currentPoint)).rgb();
    pixelDataMap.insert(currentPoint, rgb);
    drawLineTo(event->pos());
}


void ImageDisplayWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawLineTo(event->pos());
    }
}

void ImageDisplayWidget::setImage(const QImage &newImage) {
    image = newImage;
    QPixmap p = QPixmap::fromImage(image);
    setPixmap(p);
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

void ImageDisplayWidget::drawLineTo(const QPoint &endPoint) {
    QPainter painter(&image);
    painter.setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    update();
    lastPoint = endPoint;
}

void ImageDisplayWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.drawImage(QPoint(0,0), image);
}

const QString &ImageDisplayWidget::getName() const {
    return name;
}

void ImageDisplayWidget::setName(const QString &newName) {
    ImageDisplayWidget::name = newName;
}



