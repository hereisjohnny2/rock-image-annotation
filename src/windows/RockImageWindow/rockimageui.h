//
// Created by joao on 19/03/2022.
//

#ifndef ROCK_IMAGE_CPP_ROCKIMAGEUI_H
#define ROCK_IMAGE_CPP_ROCKIMAGEUI_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
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

    const static int ENTER_KEY_CODE = 16777220;
    const static int DELETE_KEY_CODE = 16777223;

    class RockImageUI : public QMainWindow {
    Q_OBJECT

    private:
        LabelData labelData {LabelData::PORE};
        QAction *showImagesAction, *showDataTablesAction;
        Ui::RockImageUI *ui;


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
        void addLayer();
        void removeCurrentLayerLayer();
        void showLayer(QTreeWidgetItem *treeWidgetItem, int column);
        void increaseWidth();
        void decreaseWidth();
        void chooseColor();

    private:
        ImageDisplaySubWindow *getSubWidowByName(const QString& name);
        ImageDisplaySubWindow *getCurrentSubWindow();
        PixelDataTable* getCurrentDataTable();
        PixelDataTable*  getPixelDataTableByName(const QString& name);
        int  getPixelDataIndexTableByName(const QString& name);
        ImageDisplayWidget *getCurrentSubWindowTopLayerImage();
        void deleteCurrentImage();
        void deleteImage(const QString& name);
        void loadImage(const QString& filePath);
        void createToolBar();
        void setActionsIcons();
        bool removeLayer(QTreeWidgetItem* item);
        bool eventFilter(QObject *obj, QEvent *event) override;
    };

} // RockImageUI

#endif //ROCK_IMAGE_CPP_ROCKIMAGEUI_H
