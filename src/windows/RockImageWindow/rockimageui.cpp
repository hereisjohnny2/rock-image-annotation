//
// Created by joao on 19/03/2022.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RockImageUI.h" resolved

#include "rockimageui.h"
#include "ui_rockimageui.h"
#include "../../widgets/PixelDataTable.h"
#include "../../widgets/ImageDisplaySubWindow.h"
#include <QtWidgets>
#include <map>

namespace RockImageUI {
    RockImageUI::RockImageUI(QWidget *parent) :
            QMainWindow(parent), ui(new Ui::RockImageUI) {
        ui->setupUi(this);

        // ImageList Events
        connect(ui->imagesList,
                SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this,
                SLOT(showImage(QListWidgetItem*)));

        // File Menu Actions
        connect(ui->openImageAction, SIGNAL(triggered()), this, SLOT(openImage()));
        connect(ui->saveDataAction, SIGNAL(triggered()), this, SLOT(saveTableData()));
        connect(ui->cleanTableAction, SIGNAL(triggered()), this, SLOT(cleanTable()));

        // Image Menu Actions
        connect(ui->applyBinarizationAction, SIGNAL(triggered()), this, SLOT(collectDataFromImage()));
    }

    RockImageUI::~RockImageUI() {
        delete ui;
    }

    void RockImageUI::openImage() {
        QString fileName = QFileDialog::getOpenFileName(this, "Abrir Imagem", "./images");
        if (fileName.isEmpty()) {
            QMessageBox::critical(this, "Error", "Não foi possível abrir a imagem selecionada.");
            return;
        }

        loadImage(fileName);
    }

    void RockImageUI::saveTableData() {
        qDebug("Save Data");

    }

    void RockImageUI::cleanTable() {
        qDebug("Clean Table");
    }

    void RockImageUI::applyBinarization() {
        qDebug("Apply Binarization");
    }

    void RockImageUI::loadImage(const QString& filePath) {
        auto *listItem = new QListWidgetItem();
        listItem->setText(filePath.section("/", -1, -1));
        listItem->setToolTip(filePath);
        ui->imagesList->addItem(listItem);

        showImage(listItem);
    }

    void RockImageUI::showImage(QListWidgetItem *listWidgetItem) {
        QString filePath = listWidgetItem->toolTip();
        QString fileName = listWidgetItem->text();

        auto *pixelDataTable = new PixelDataTable();
        ui->dataTablesTab->addTab(pixelDataTable, fileName);

        auto *subWindow = new ImageDisplaySubWindow(filePath, fileName);
        subWindow->setAttribute(Qt::WA_DeleteOnClose);
        ui->openImagesArea->addSubWindow(subWindow);

        subWindow->loadImage(filePath);
        subWindow->show();
    }

    void RockImageUI::collectDataFromImage() {
        auto *activeSubWindow = ui->openImagesArea->currentSubWindow();
        auto *imageDisplayWidget = (ImageDisplayWidget *)(activeSubWindow->widget());

        if (imageDisplayWidget == nullptr) {
            return;
        }
    }
} // RockImageUI
