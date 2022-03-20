//
// Created by joao on 19/03/2022.
//

#include "PixelData.h"

int PixelData::getPosX() const {
    return posX;
}

void PixelData::setPosX(int posX) {
    PixelData::posX = posX;
}

int PixelData::getPosY() const {
    return posY;
}

void PixelData::setPosY(int posY) {
    PixelData::posY = posY;
}

int PixelData::getRed() const {
    return red;
}

void PixelData::setRed(int red) {
    PixelData::red = red;
}

int PixelData::getGreen() const {
    return green;
}

void PixelData::setGreen(int green) {
    PixelData::green = green;
}

int PixelData::getBlue() const {
    return blue;
}

void PixelData::setBlue(int blue) {
    PixelData::blue = blue;
}

int PixelData::getLabel() const {
    return label;
}

void PixelData::setLabel(int label) {
    PixelData::label = label;
}
