#include "StackedImagesWidget.h"
#include <algorithm>

StackedImagesWidget::StackedImagesWidget() {
    stackedLayout = new QStackedLayout;
    addLayout(stackedLayout);
}

QStack<ImageDisplayWidget *> StackedImagesWidget::getImages() const {
    return layers;
}

ImageDisplayWidget *StackedImagesWidget::getImageByName(const QString &name) const {
    auto layer = std::find_if(layers.begin(), layers.end(), [&](ImageDisplayWidget* item) {
        return name == item->getLabel();
    });

    return *layer;
}

void StackedImagesWidget::addLayer(ImageDisplayWidget *layer) {
    stackedLayout->addWidget(layer);
    stackedLayout->setCurrentIndex(stackedLayout->count() - 1);
    layers.push_back(layer);
}

void StackedImagesWidget::scaleImage(double factor) const {
    auto image = getImageByName("baseImage");
    image->resize(factor * image->pixmap(Qt::ReturnByValue).size());
}

void StackedImagesWidget::removeLayer() {
    if (stackedLayout->count() > 1) {
        layers.pop_back();
        stackedLayout->takeAt(stackedLayout->count() - 1);
        stackedLayout->setCurrentIndex(stackedLayout->count() - 1);
    }
}
