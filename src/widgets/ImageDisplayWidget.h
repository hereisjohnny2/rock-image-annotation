//
// Created by joao on 19/03/2022.
//

#ifndef ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H
#define ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H


#include <QLabel>
#include "../entities/PixelData.h"

class ImageDisplayWidget : public QLabel {
private:
    QImage image;
    PixelData collectedPixelData = PixelData();
public:
    ImageDisplayWidget();
    void openImage(const QString& filePath);
    const PixelData &getCollectedPixelData() const;
    void resetCollectedData();
    void mouseMoveEvent(QMouseEvent *ev) override;
};


#endif //ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H
