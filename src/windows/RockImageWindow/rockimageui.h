//
// Created by joao on 19/03/2022.
//

#ifndef ROCK_IMAGE_CPP_ROCKIMAGEUI_H
#define ROCK_IMAGE_CPP_ROCKIMAGEUI_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "../../widgets/PixelDataTable.h"
#include "../../widgets/ImageDisplaySubWindow.h"

namespace RockImageUI {
    enum LabelData {
        SOLID,
        PORE
    };

    QT_BEGIN_NAMESPACE
    namespace Ui { class RockImageUI; }
    QT_END_NAMESPACE

    class RockImageUI : public QMainWindow {
    Q_OBJECT

    private:
        LabelData labelData {LabelData::PORE};

    public:
        explicit RockImageUI(QWidget *parent = nullptr);

        ~RockImageUI() override;

    private slots:
        void openImage();
        void saveTableData();
        void cleanTable();
        void applyBinarization();
        void collectDataFromImage();
        void showImage(QListWidgetItem *listWidgetItem);
        void closeAllWindows();
        void zoomIn();
        void zoomOut();
        void changeTargetLabel();

    private:
        void loadImage(const QString& filePath);
        int getPixelDataTableByName(const QString& qString);
        ImageDisplaySubWindow *getSubWidowByName(const QString& name);

        void createToolBar();
        void setActionsIcons();

        Ui::RockImageUI *ui;
    };

} // RockImageUI

#endif //ROCK_IMAGE_CPP_ROCKIMAGEUI_H
