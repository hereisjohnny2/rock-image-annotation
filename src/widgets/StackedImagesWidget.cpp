//
// Created by joao on 26/03/2022.
//

#include "StackedImagesWidget.h"
#include <algorithm>

StackedImagesWidget::StackedImagesWidget() {
    stackedLayout = new QStackedLayout;
    addLayout(stackedLayout);
}

QList<ImageDisplayWidget *> StackedImagesWidget::getImages() const {
    return layers;
}

ImageDisplayWidget *StackedImagesWidget::getImageByName(const QString &name) const {
    auto layer = std::find_if(layers.begin(), layers.end(), [&](ImageDisplayWidget* item) {
        return name == item->getName();
    });

    return *layer;
}

void StackedImagesWidget::addLayer(ImageDisplayWidget *layer) {
    stackedLayout->addWidget(layer);
    layers.push_back(layer);
}

void StackedImagesWidget::scaleImage(double factor) {
    auto image = getImageByName("baseImage");
    image->resize(factor * image->pixmap(Qt::ReturnByValue).size());
}

