//
// Created by joao on 18/04/2022.
//

#ifndef ROCK_IMAGE_CPP_COLORDIALOG_H
#define ROCK_IMAGE_CPP_COLORDIALOG_H


#include <QDialog>
#include <QSpinBox>
#include <QFormLayout>


class ColorDialog : public QDialog {
    Q_OBJECT
public:
    explicit ColorDialog(QWidget *parent = nullptr);

    static QList<int> getStrings(QWidget *parent, bool *ok = nullptr);

private:
    QFormLayout *lytMain;
    QList<QSpinBox*> fields;
    void createColorInput(const QString &label);

};


#endif //ROCK_IMAGE_CPP_COLORDIALOG_H
