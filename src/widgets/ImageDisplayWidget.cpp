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
    if (label == "baseImage") {
        return;
    }

    QPoint currentPoint = event->pos();
    QRgb rgb = QColor(image.pixel(currentPoint)).rgb();
    pixelDataMap.insert(currentPoint, rgb);
    drawLineTo(event->pos());
}


void ImageDisplayWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton and label != "baseImage") {
        drawLineTo(event->pos());
    }
}

void ImageDisplayWidget::setImage(const QImage &newImage) {
    image = newImage;
    compositeImage = createImageWithOverlay();
    QPixmap p = QPixmap::fromImage(compositeImage);
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
    QPainter painter(&compositeImage);
    painter.setPen(QPen(penBrush, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    int rad = (10 / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                   .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void ImageDisplayWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.drawImage(QPoint(0,0), compositeImage);
}

const QString &ImageDisplayWidget::getLabel() const {
    return label;
}

void ImageDisplayWidget::setLabel(const QString &name) {
    ImageDisplayWidget::label = name;
}

QImage ImageDisplayWidget::createImageWithOverlay() {
    QImage imageWithOverlay = QImage(image.size(), QImage::Format_RGB16);
    QPainter painter(&imageWithOverlay);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(imageWithOverlay.rect(), Qt::transparent);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, image);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, image.createAlphaMask());

    painter.end();

    return imageWithOverlay;
}

int ImageDisplayWidget::getPenWidth() const {
    return penWidth;
}

void ImageDisplayWidget::setPenWidth(int penWidth) {
    ImageDisplayWidget::penWidth = penWidth;
}

QBrush ImageDisplayWidget::getPenBrush() const {
    return penBrush;
}

void ImageDisplayWidget::setPenBrush(const QBrush &penBrush) {
    ImageDisplayWidget::penBrush = penBrush;
}



