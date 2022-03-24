//
// Created by joao on 24/03/2022.
//

#ifndef ROCK_IMAGE_CPP_CUSTOMMESSAGEDIALOGS_H
#define ROCK_IMAGE_CPP_CUSTOMMESSAGEDIALOGS_H


#include <QString>

class CustomMessageDialogs {
public:
    static bool Question(QWidget *parent, const QString& title, const QString& text);
};


#endif //ROCK_IMAGE_CPP_CUSTOMMESSAGEDIALOGS_H
