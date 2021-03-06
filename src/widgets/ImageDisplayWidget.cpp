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

        if (currentLayer == BASE_IMAGE) return;

        if (pixelDataMap.contains(currentPoint)) {
            QRgb rgb = QColor(image.pixel(currentPoint)).rgb();

            int rad = penWidth / 2;

            for (int i = currentPoint.x() - rad; i < currentPoint.x() + rad; ++i)
                for (int j = currentPoint.y() - rad; j < currentPoint.y() + rad; ++j)
                    pixelDataMap.insert(QPoint(i, j), {rgb, currentLayer});
        }

        drawLineTo(event->pos());
    }


    void ImageDisplayWidget::mouseReleaseEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton and currentLayer != BASE_IMAGE) {
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
        pixelDataMap.clear();
    }

    void ImageDisplayWidget::drawLineTo(const QPoint &endPoint) {
        QPen pen(penBrush, penWidth,
                 Qt::SolidLine,
                 Qt::RoundCap,
                 Qt::RoundJoin);
        QPainter painter(&compositeImage);
        painter.setPen(pen);
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
        setCurrentLayer(BASE_IMAGE);

        for (int i = 0; i < compositeImage.width(); ++i) {
            for (int j = 0; j < compositeImage.height(); ++j) {
                if (compositeImage.pixel(i, j) == penBrush.color().rgb()) {
                    auto imagePixel = image.pixel(i, j);
                    compositeImage.setPixel(i, j, imagePixel);
                    pixelDataMap.remove(QPoint(i, j));
                }
            }
        }

        pixelDataMap.removeIf([&](const QHash<QPoint, RGBLayerName>::iterator &it){
            return it.value().layerName == layerName;
        });
    }
}



