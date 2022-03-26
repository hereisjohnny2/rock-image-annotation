#ifndef ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H
#define ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H


#include <QLabel>
#include <map>
#include "PixelDataTable.h"

class ImageDisplayWidget : public QLabel {
private:
    QImage image;
    QImage compositeImage;
    QHash<QPoint, QRgb> pixelDataMap{};
    QPoint lastPoint;
    QString label = "layer";

private:
    void drawLineTo(const QPoint &endPoint);
    QImage createImageWithOverlay();

public:
    ImageDisplayWidget();
    void setImage(const QImage &newImage);
    [[nodiscard]] const QImage &getImage() const;
    [[nodiscard]] const QHash<QPoint, QRgb> &getPixelDataMap() const;
    [[nodiscard]] const QString &getLabel() const;
    void setLabel(const QString &name);
    void clearPixelDataMap();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};


#endif //ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H
