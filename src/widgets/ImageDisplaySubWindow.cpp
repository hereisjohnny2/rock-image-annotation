//
// Created by joao on 19/03/2022.
//

#include <QImageReader>
#include <QMessageBox>
#include <QGuiApplication>
#include <QDir>
#include "ImageDisplaySubWindow.h"
#include "ImageDisplayWidget.h"

ImageDisplaySubWindow::ImageDisplaySubWindow(const QString& filePath, const QString& fileName)
    : imageLabel(new QLabel()), scrollArea(new QScrollArea())
{
    this->setWindowTitle(fileName);

    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);

    this->setWidget(scrollArea);
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

    image = newImage;
    imageLabel->setPixmap(QPixmap::fromImage(image));

    imageLabel->adjustSize();

    scaleFactor = 1.0;
    scrollArea->setVisible(true);

    return true;
}

