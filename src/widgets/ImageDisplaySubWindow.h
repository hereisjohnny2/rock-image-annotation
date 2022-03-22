#ifndef ROCK_IMAGE_CPP_IMAGEDISPLAYSUBWINDOW_H
#define ROCK_IMAGE_CPP_IMAGEDISPLAYSUBWINDOW_H


#include <QMdiSubWindow>
#include <QLabel>
#include <QScrollArea>
#include "ImageDisplayWidget.h"

class ImageDisplaySubWindow : public QMdiSubWindow {

private:
    ImageDisplayWidget *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor = 1;

public:
    ImageDisplaySubWindow(const QString& filePath, const QString& fileName);

    [[nodiscard]] ImageDisplayWidget *getImageLabel() const;

    bool loadImage(const QString &filePath);
    void scaleImage(double factor);

private:
    static void adjustScrollBar(QScrollBar *bar, double factor);
};


#endif //ROCK_IMAGE_CPP_IMAGEDISPLAYSUBWINDOW_H
