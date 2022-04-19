#ifndef ROCK_IMAGE_CPP_IMAGEDISPLAYSUBWINDOW_H
#define ROCK_IMAGE_CPP_IMAGEDISPLAYSUBWINDOW_H


#include <QMdiSubWindow>
#include <QLabel>
#include <QScrollArea>
#include "ImageDisplayWidget.h"
#include "StackedImagesWidget.h"

class ImageDisplaySubWindow : public QMdiSubWindow {

private:
    StackedImagesWidget *stackedImagesWidget;
    QScrollArea *scrollArea;
    double scaleFactor = 1;
    static void adjustScrollBar(QScrollBar *bar, double factor);
    static QColor generateRandomColor();

public:
    ImageDisplaySubWindow(const QString& filePath, const QString& fileName);
    [[nodiscard]] ImageDisplayWidget *getTopLayerImage() const;
    bool loadImage(const QString &filePath);
    void scaleImage(double factor);
    void addNewLayer(const QString& label);
    void showLayer(const QString &name);
    void removeCurrentLayer();
    void removeLayerByName(const QString& name);
    void updatePenWidth(const int& value);
    void updatePenBrush(const QColor & value);

};


#endif //ROCK_IMAGE_CPP_IMAGEDISPLAYSUBWINDOW_H
