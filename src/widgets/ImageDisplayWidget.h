//
// Created by joao on 19/03/2022.
//

#ifndef ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H
#define ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H


#include <QLabel>
#include <map>
#include "PixelDataTable.h"

class ImageDisplayWidget : public QLabel {
private:
    QImage image;
    QHash<QPoint, QRgb> pixelDataMap{};

public:
    ImageDisplayWidget();

    void setImage(const QImage &newImage);

    [[nodiscard]] const QImage &getImage() const;
    const QHash<QPoint, QRgb> &getPixelDataMap() const;

    void mouseMoveEvent(QMouseEvent *mouseEvent) override;
};


#endif //ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H
