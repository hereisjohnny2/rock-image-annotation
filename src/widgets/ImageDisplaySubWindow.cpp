#include <QImageReader>
#include <QMessageBox>
#include <QGuiApplication>
#include <QDir>
#include <QScreen>
#include <QMouseEvent>
#include <QScrollBar>
#include <QPainter>

#include "ImageDisplaySubWindow.h"
#include "ImageDisplayWidget.h"

ImageDisplaySubWindow::ImageDisplaySubWindow(const QString& filePath, const QString& fileName)
    : stackedImagesWidget(new StackedImagesWidget()), scrollArea(new QScrollArea())
{
    this->setWindowTitle(fileName);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setLayout(stackedImagesWidget);
    scrollArea->setVisible(false);

    this->setWidget(scrollArea);

    resize(QGuiApplication::primaryScreen()->availableSize() * 2 / 5);
}

bool ImageDisplaySubWindow::loadImage(const QString &filePath) {
    QImageReader imageReader(filePath);
    imageReader.setAutoTransform(true);
    const QImage newImage = imageReader.read();

    if (newImage.isNull()) {
        QMessageBox::information(
                this,
                QGuiApplication::applicationDisplayName(),
                tr("Cannot load %1: %2")
                    .arg(QDir::toNativeSeparators(filePath), imageReader.errorString()));
        return false;
    }

    auto *layer = new ImageDisplayWidget();
    layer->setImage(newImage);
    layer->setLabel("baseImage");

    stackedImagesWidget->addLayer(layer);

    scaleFactor = 1.0;
    scrollArea->setVisible(true);

    return true;
}

ImageDisplayWidget *ImageDisplaySubWindow::getTopLayerImage() const {
    return stackedImagesWidget->getImages().top();
}

void ImageDisplaySubWindow::scaleImage(double factor) {
    scaleFactor *= factor;
    stackedImagesWidget->scaleImage(scaleFactor);

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);
}

void ImageDisplaySubWindow::adjustScrollBar(QScrollBar *bar, double factor) {
    bar->setValue(int(factor * bar->value()
                      + ((factor - 1) * bar->pageStep() / 2)));

}

void ImageDisplaySubWindow::addNewLayer(const QString& label) {
    auto layer = new ImageDisplayWidget();
    auto baseImage = stackedImagesWidget->getImages().top()->getImage();
    layer->setLabel(label);
    layer->setImage(baseImage);
    stackedImagesWidget->addLayer(layer);
}

void ImageDisplaySubWindow::removeCurrentLayer() {
    stackedImagesWidget->removeLayer();
}

void ImageDisplaySubWindow::showLayer(const QString &name) {
    stackedImagesWidget->setTopLayer(name);
}

void ImageDisplaySubWindow::removeLayerByName(const QString& name) {
    stackedImagesWidget->removeLayerByName(name);
}

