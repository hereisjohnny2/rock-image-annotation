#include <QMouseEvent>
#include <QPainter>
#include "ImageDisplayWidget.h"

namespace ImageDisplayWidget {
    ImageDisplayWidget::ImageDisplayWidget() {
        setBackgroundRole(QPalette::Dark);
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

        if (pixelDataMap.contains(currentPoint) or currentLayer == "baseImage") return;

        QRgb rgb = QColor(image.pixel(currentPoint)).rgb();

        pixelDataMap.insert(currentPoint, { rgb, currentLayer });
        drawLineTo(event->pos());
    }


    void ImageDisplayWidget::mouseReleaseEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton and currentLayer != "baseImage") {
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

    const QHash<QPoint, RGBLayerName> &ImageDisplayWidget::getPixelDataMap() const {
        return pixelDataMap;
    }

    void ImageDisplayWidget::clearPixelDataMap() {
        pixelDataMap = {};
    }

    void ImageDisplayWidget::drawLineTo(const QPoint &endPoint) {
        QPainter painter(&compositeImage);
        painter.setPen(QPen(penBrush, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(lastPoint, endPoint);
        int rad = (penWidth / 2) + 2;
        update(QRect(lastPoint, endPoint).normalized()
                       .adjusted(-rad, -rad, +rad, +rad));
        lastPoint = endPoint;
    }

    void ImageDisplayWidget::paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, false);
        painter.drawImage(QPoint(0, 0), compositeImage);
        update();
    }

    const QString &ImageDisplayWidget::getCurrentLayer() const {
        return currentLayer;
    }

    void ImageDisplayWidget::setCurrentLayer(const QString &layerName) {
        currentLayer = layerName;
    }

    QImage ImageDisplayWidget::createImageWithOverlay() {
        QImage imageWithOverlay = QImage(image.size(), image.format());
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

    void ImageDisplayWidget::setPenWidth(int newPenWidth) {
        penWidth = newPenWidth;
    }

    void ImageDisplayWidget::setPenBrush(const QBrush &newPenBrush) {
        penBrush = newPenBrush;
    }

    void ImageDisplayWidget::resizeEvent(QResizeEvent *event) {
        QWidget::resizeEvent(event);
        resizeImage();
    }

    void ImageDisplayWidget::resizeImage() {
        image = image.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        compositeImage = compositeImage.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        update();
    }

    const QBrush &ImageDisplayWidget::getPenBrush() const {
        return penBrush;
    }

    void ImageDisplayWidget::removeLayer(const QString &layerName) {
        setCurrentLayer("baseImage");
    }
}



