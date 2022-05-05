#include <QImageReader>
#include <QMessageBox>
#include <QGuiApplication>
#include <QDir>
#include <QScreen>
#include <QMouseEvent>
#include <QScrollBar>
#include <QPainter>
#include <random>

#include "ImageDisplaySubWindow.h"
#include "ImageDisplayWidget.h"

ImageDisplaySubWindow::ImageDisplaySubWindow(const QString& filePath, const QString& fileName)
    : scrollArea(new QScrollArea())
{
    this->setWindowTitle(fileName);

    scrollArea->setBackgroundRole(QPalette::Dark);
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

    imageDisplayWidget = new ImageDisplayWidget::ImageDisplayWidget();
    imageDisplayWidget->setImage(newImage);
    imageDisplayWidget->setCurrentLayer("baseImage");

    scrollArea->setWidget(imageDisplayWidget);
    scrollArea->setVisible(true);

    scaleFactor = 1.0;
    return true;
}

void ImageDisplaySubWindow::scaleImage(double factor) {
    if (imageDisplayWidget == nullptr) return;

    scaleFactor *= factor;

    imageDisplayWidget->resize(scaleFactor * imageDisplayWidget->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);
}

void ImageDisplaySubWindow::adjustScrollBar(QScrollBar *bar, double factor) {
    bar->setValue(int(factor * bar->value()
                      + ((factor - 1) * bar->pageStep() / 2)));

}

bool ImageDisplaySubWindow::addNewLayer(const QString& label) {
    if (imageDisplayWidget == nullptr) return false;

    if (layersColors.contains(label)) {
        QMessageBox::warning(
                this,
                QGuiApplication::applicationDisplayName(),
                tr("Layer with name %1 already exists!").arg(label));
        return false;
    }

    QBrush brush(generateRandomColor());
    layersColors.emplace(label, brush);

    imageDisplayWidget->setCurrentLayer(label);
    imageDisplayWidget->setPenBrush(brush);

    return true;
}

void ImageDisplaySubWindow::setCurrentLayer(const QString &layerName) {
    if (imageDisplayWidget == nullptr) return;

    imageDisplayWidget->setCurrentLayer(layerName);
    imageDisplayWidget->setPenBrush(layersColors.value(layerName));
}

void ImageDisplaySubWindow::removeLayerByName(const QString& layerName) {
    layersColors.remove(layerName);
}

QColor ImageDisplaySubWindow::generateRandomColor() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> randomInt(0,255);

    return {
        static_cast<int>(randomInt(rng)),
        static_cast<int>(randomInt(rng)),
        static_cast<int>(randomInt(rng))
    };
}

void ImageDisplaySubWindow::updatePenWidth(const int &value) {
    if (imageDisplayWidget == nullptr) return;

    int currentWidth = imageDisplayWidget->getPenWidth();
    imageDisplayWidget->setPenWidth(currentWidth + value);
}

void ImageDisplaySubWindow::updatePenBrush(const QColor &value) {
    if (imageDisplayWidget == nullptr) return;

    imageDisplayWidget->setPenBrush(value);
}

ImageDisplayWidget::ImageDisplayWidget *ImageDisplaySubWindow::getImageDisplayWidget() const {
    return imageDisplayWidget;
}

void ImageDisplaySubWindow::removeCurrentLayer() {
    if (imageDisplayWidget == nullptr) return;

    layersColors.remove(imageDisplayWidget->getCurrentLayer());
    setCurrentLayer("baseImage");
}
