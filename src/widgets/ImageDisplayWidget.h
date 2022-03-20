//
// Created by joao on 19/03/2022.
//

#ifndef ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H
#define ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H


#include <QLabel>
#include <map>

class ImageDisplayWidget : public QLabel {
private:
    QImage image;

public:
    ImageDisplayWidget();

    void setImage(const QImage &newImage);

    [[nodiscard]] const QImage &getImage() const;

    void mouseMoveEvent(QMouseEvent *mouseEvent) override;
};


#endif //ROCK_IMAGE_CPP_IMAGEDISPLAYWIDGET_H
