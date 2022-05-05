#ifndef ROCK_IMAGE_CPP_IMAGEDISPLAYSUBWINDOW_H
#define ROCK_IMAGE_CPP_IMAGEDISPLAYSUBWINDOW_H


#include <QMdiSubWindow>
#include <QLabel>
#include <QScrollArea>
#include "ImageDisplayWidget.h"

class ImageDisplaySubWindow : public QMdiSubWindow {

private:
    QHash<QString, QBrush> layersColors{};
    ImageDisplayWidget::ImageDisplayWidget* imageDisplayWidget{};
    QScrollArea *scrollArea;
    double scaleFactor{1.0};

    static QColor generateRandomColor();
    static void adjustScrollBar(QScrollBar *bar, double factor);

public:
    ImageDisplaySubWindow(const QString& filePath, const QString& fileName);

    [[nodiscard]] ImageDisplayWidget::ImageDisplayWidget *getImageDisplayWidget() const;

    bool loadImage(const QString &filePath);
    void scaleImage(double factor);
    bool addNewLayer(const QString& label);

    void setCurrentLayer(const QString &layerName);
    void removeLayerByName(const QString& layerName);
    void removeCurrentLayer();

    void updatePenWidth(const int& value);
    void updatePenBrush(const QColor & value);
};


#endif //ROCK_IMAGE_CPP_IMAGEDISPLAYSUBWINDOW_H
