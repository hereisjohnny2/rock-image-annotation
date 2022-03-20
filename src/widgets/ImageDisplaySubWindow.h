//
// Created by joao on 19/03/2022.
//

#ifndef ROCK_IMAGE_CPP_IMAGEDISPLAYSUBWINDOW_H
#define ROCK_IMAGE_CPP_IMAGEDISPLAYSUBWINDOW_H


#include <QMdiSubWindow>
#include <QLabel>
#include <QScrollArea>

class ImageDisplaySubWindow : public QMdiSubWindow {

public:
    ImageDisplaySubWindow(const QString& filePath, const QString& fileName);
    bool loadImage(const QString &filePath);

private:
    QImage image;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor = 1;
};


#endif //ROCK_IMAGE_CPP_IMAGEDISPLAYSUBWINDOW_H
