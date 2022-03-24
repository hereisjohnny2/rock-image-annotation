//
// Created by joao on 24/03/2022.
//

#include <QMessageBox>
#include "CustomMessageDialogs.h"

bool CustomMessageDialogs::Question(QWidget *parent, const QString &title, const QString &text) {
    QMessageBox::StandardButton result = QMessageBox::question(parent, title, text);
    if (result == QMessageBox::Yes) {
        return true;
    }
    return false;
}
