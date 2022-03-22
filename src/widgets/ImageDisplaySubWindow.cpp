#include <QImageReader>
#include <QMessageBox>
#include <QGuiApplication>
#include <QDir>
#include <QScreen>
#include <QMouseEvent>
#include <QScrollBar>

#include "ImageDisplaySubWindow.h"
#include "ImageDisplayWidget.h"

ImageDisplaySubWindow::ImageDisplaySubWindow(const QString& filePath, const QString& fileName)
    : imageLabel(new ImageDisplayWidget()), scrollArea(new QScrollArea())
{
    this->setWindowTitle(fileName);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
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

    imageLabel->setImage(newImage);

    scaleFactor = 1.0;
    scrollArea->setVisible(true);

    return true;
}

ImageDisplayWidget *ImageDisplaySubWindow::getImageLabel() const {
    return imageLabel;
}

void ImageDisplaySubWindow::scaleImage(double factor) {
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);
}

void ImageDisplaySubWindow::adjustScrollBar(QScrollBar *bar, double factor) {
    bar->setValue(int(factor * bar->value()
                      + ((factor - 1) * bar->pageStep() / 2)));

}

