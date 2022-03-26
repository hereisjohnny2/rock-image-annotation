//
// Created by joao on 26/03/2022.
//

#ifndef ROCK_IMAGE_CPP_STACKEDIMAGESWIDGET_H
#define ROCK_IMAGE_CPP_STACKEDIMAGESWIDGET_H


#include <QLabel>
#include <QStackedLayout>
#include <QStack>
#include "ImageDisplayWidget.h"

class StackedImagesWidget : public QVBoxLayout  {
private:
    QStackedLayout *stackedLayout;
    QStack<ImageDisplayWidget *> layers;

public:
    StackedImagesWidget();
    [[nodiscard]] QStack<ImageDisplayWidget *> getImages() const;
    [[nodiscard]] ImageDisplayWidget * getImageByName(const QString &name) const;
    void addLayer(ImageDisplayWidget *layer);
    void removeLayer();
    void scaleImage(double factor);
};


#endif //ROCK_IMAGE_CPP_STACKEDIMAGESWIDGET_H
