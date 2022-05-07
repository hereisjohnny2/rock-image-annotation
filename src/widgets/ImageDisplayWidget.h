#ifndef ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H
#define ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H


#include <QLabel>
#include <map>
#include <utility>
#include "PixelDataTable.h"

namespace ImageDisplayWidget {
    const QString BASE_IMAGE = "baseImage";

    struct RGBLayerName {
        QRgb rgb{};
        QString layerName;

        RGBLayerName() = default;
        RGBLayerName(QRgb pRgb, QString pLayerName) : rgb(pRgb), layerName(std::move(pLayerName)) {}
    };

    class ImageDisplayWidget : public QLabel {
    public:

    private:
        QImage image{};
        QImage compositeImage{};

        QHash<QPoint, RGBLayerName> pixelDataMap{};
        QPoint lastPoint{};
        QString currentLayer{BASE_IMAGE};

        QBrush penBrush{Qt::blue};
        int penWidth{10};

    private:
        void drawLineTo(const QPoint &endPoint);

        QImage createImageWithOverlay();

    public:
        ImageDisplayWidget();

        [[nodiscard]] const QHash<QPoint, RGBLayerName> &getPixelDataMap() const;

        [[nodiscard]] const QString &getCurrentLayer() const;

        [[nodiscard]] int getPenWidth() const;

        [[nodiscard]] const QBrush &getPenBrush() const;

        void setImage(const QImage &newImage);

        void setPenWidth(int newPenWidth);

        void setPenBrush(const QBrush &newPenBrush);

        void setCurrentLayer(const QString &layerName);

        void clearPixelDataMap();

        void resizeImage();

        void removeLayer(const QString &layerName);

    protected:
        void mousePressEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void paintEvent(QPaintEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;
    };
}

#endif //ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H
