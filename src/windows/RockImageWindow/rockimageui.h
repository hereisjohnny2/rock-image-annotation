//
// Created by joao on 19/03/2022.
//

#ifndef ROCK_IMAGE_CPP_ROCKIMAGEUI_H
#define ROCK_IMAGE_CPP_ROCKIMAGEUI_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace RockImageUI {
    QT_BEGIN_NAMESPACE
    namespace Ui { class RockImageUI; }
    QT_END_NAMESPACE

    class RockImageUI : public QMainWindow {
    Q_OBJECT

    public:
        explicit RockImageUI(QWidget *parent = nullptr);

        ~RockImageUI() override;

    private slots:
        void openImage();
        void saveTableData();
        void cleanTable();
        void applyBinarization();
        void collectDataFromImage();

    private:
        void loadImage(const QString& filePath);
        void showImage(const QListWidgetItem &listWidgetItem);

    private:
        Ui::RockImageUI *ui;
    };

} // RockImageUI

#endif //ROCK_IMAGE_CPP_ROCKIMAGEUI_H
