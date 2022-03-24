#include <QApplication>
#include <QPushButton>
#include "windows/RockImageWindow/rockimageui.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    RockImageUI::RockImageUI rockImageUi;
    rockImageUi.show();

    return QApplication::exec();
}
