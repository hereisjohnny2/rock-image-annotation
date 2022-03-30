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
    void scaleImage(double factor) const;
    void setTopLayer(const QString &name);
    void removeLayerByName(const QString &name);
    static const QString BASE_IMAGE;
};


#endif //ROCK_IMAGE_CPP_STACKEDIMAGESWIDGET_H
