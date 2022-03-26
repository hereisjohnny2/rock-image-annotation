//
// Created by joao on 26/03/2022.
//

#ifndef ROCK_IMAGE_CPP_STACKEDIMAGESWIDGET_H
#define ROCK_IMAGE_CPP_STACKEDIMAGESWIDGET_H


#include <QLabel>
#include <QStackedLayout>
#include "ImageDisplayWidget.h"

class StackedImagesWidget : public QVBoxLayout  {
private:
    QStackedLayout *stackedLayout;
    QList<ImageDisplayWidget *> layers;

public:
    StackedImagesWidget();
    [[nodiscard]] QList<ImageDisplayWidget *> getImages() const;
    [[nodiscard]] ImageDisplayWidget * getImageByName(const QString &name) const;
    void addLayer(ImageDisplayWidget *layer);
    void scaleImage(double factor);
};


#endif //ROCK_IMAGE_CPP_STACKEDIMAGESWIDGET_H
