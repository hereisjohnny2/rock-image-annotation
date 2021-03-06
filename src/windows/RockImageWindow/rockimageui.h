#ifndef ROCK_IMAGE_CPP_ROCKIMAGEUI_H
#define ROCK_IMAGE_CPP_ROCKIMAGEUI_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include "../../widgets/PixelDataTable.h"
#include "../../widgets/ImageDisplaySubWindow.h"

namespace RockImageUI {
    QT_BEGIN_NAMESPACE
    namespace Ui { class RockImageUI; }
    QT_END_NAMESPACE

    const static int ENTER_KEY_CODE = 16777220;
    const static int DELETE_KEY_CODE = 16777223;

    class RockImageUI : public QMainWindow {
    Q_OBJECT

    public:
        explicit RockImageUI(QWidget *parent = nullptr);

        ~RockImageUI() override;

    private:
        QAction *showImagesAction, *showDataTablesAction;
        Ui::RockImageUI *ui;

    private slots:

        void openImage();

        void saveTableData();

        void cleanTable();

        void collectDataFromImage();

        void showImage(QListWidgetItem *listWidgetItem);

        void closeAllWindows();

        void zoomIn();

        void zoomOut();

        void addLayer();

        void showLayer(QTreeWidgetItem *treeWidgetItem, int column);

        void increaseWidth();

        void decreaseWidth();

        void chooseColor();

    private:
        ImageDisplaySubWindow *getSubWidowByName(const QString &name);

        ImageDisplaySubWindow *getCurrentSubWindow();

        PixelDataTable *getCurrentDataTable();

        PixelDataTable *getPixelDataTableByName(const QString &name);

        int getPixelDataIndexTableByName(const QString &name);

        void deleteCurrentImage();

        void deleteImage(const QString &name);

        void loadImage(const QString &filePath);

        void createToolBar();

        void setActionsIcons();

        bool removeLayer(QTreeWidgetItem *item);

        bool eventFilter(QObject *obj, QEvent *event) override;

        static QPair<QString, QString> getLayerAndSubWindowName(QTreeWidgetItem *treeWidgetItem, int column);
    };

} // RockImageUI

#endif //ROCK_IMAGE_CPP_ROCKIMAGEUI_H
