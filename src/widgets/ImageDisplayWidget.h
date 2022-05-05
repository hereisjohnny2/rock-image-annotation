#ifndef ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H
#define ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H


#include <QLabel>
#include <map>
#include "PixelDataTable.h"

namespace ImageDisplayWidget {
    class ImageDisplayWidget : public QLabel {
    public:

    private:
        QImage image{};
        QImage compositeImage{};

        QHash<QPoint, QRgb> pixelDataMap{};
        QPoint lastPoint{};
        QString currentLayer{""};

        QBrush penBrush{Qt::blue};
        int penWidth{10};

    private:
        void drawLineTo(const QPoint &endPoint);

        QImage createImageWithOverlay();

    public:
        ImageDisplayWidget();

        [[nodiscard]] const QHash<QPoint, QRgb> &getPixelDataMap() const;

        [[nodiscard]] const QString &getCurrentLayer() const;

        [[nodiscard]] int getPenWidth() const;

        [[nodiscard]] const QBrush &getPenBrush() const;

        void setImage(const QImage &newImage);

        void setPenWidth(int newPenWidth);

        void setPenBrush(const QBrush &newPenBrush);

        void setCurrentLayer(const QString &layerName);

        void clearPixelDataMap();

        void resizeImage();

    protected:
        void mousePressEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void paintEvent(QPaintEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;
    };
}

#endif //ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H
