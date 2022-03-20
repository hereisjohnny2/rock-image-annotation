//
// Created by joao on 19/03/2022.
//

#ifndef ROCK_IMAGE_CPP_PIXELDATA_H
#define ROCK_IMAGE_CPP_PIXELDATA_H


class PixelData {
public:
    int getPosX() const;

    void setPosX(int posX);

    int getPosY() const;

    void setPosY(int posY);

    int getRed() const;

    void setRed(int red);

    int getGreen() const;

    void setGreen(int green);

    int getBlue() const;

    void setBlue(int blue);

    int getLabel() const;

    void setLabel(int label);

private:
    int posX = 0, posY = 0, red = 0, green = 0, blue = 0, label = 0;
};


#endif //ROCK_IMAGE_CPP_PIXELDATA_H
