#include "rockimageui.h"
#include "ui_rockimageui.h"
#include <QtWidgets>
#include <map>
#include <QDialog>

namespace RockImageUI {
    RockImageUI::RockImageUI(QWidget *parent) :
            QMainWindow(parent), ui(new Ui::RockImageUI) {
        ui->setupUi(this);

        setActionsIcons();

        // ImageList Events
        connect(ui->imagesList,
                SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this,
                SLOT(showImage(QListWidgetItem*)));

        // File Menu Actions
        connect(ui->openImageAction, SIGNAL(triggered()), this, SLOT(openImage()));
        connect(ui->saveDataAction, SIGNAL(triggered()), this, SLOT(saveTableData()));
        connect(ui->cleanTableAction, SIGNAL(triggered()), this, SLOT(cleanTable()));
        connect(ui->exitAction, &QAction::triggered, [this](){QApplication::quit();});

        // ToolBar Actions
        connect(ui->collectDataAction, SIGNAL(triggered()), this, SLOT(collectDataFromImage()));
        connect(ui->closeAllAction, SIGNAL(triggered()), this, SLOT(closeAllWindows()));
        connect(ui->zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));
        connect(ui->zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));
        connect(ui->changeLabelAction, SIGNAL(triggered()), this, SLOT(changeTargetLabel()));

        createToolBar();
    }

    RockImageUI::~RockImageUI() {
        delete ui;
    }

    void RockImageUI::openImage() {
        QString fileName = QFileDialog::getOpenFileName(this, "Abrir Imagem");
        if (fileName.isEmpty()) {
            QMessageBox::critical(this, "Error", "Não foi possível abrir a imagem selecionada.");
            return;
        }

        loadImage(fileName);
    }

    void RockImageUI::saveTableData() {
        auto *pixelDataTable = (PixelDataTable*) ui->dataTablesTab->currentWidget();
        if (pixelDataTable == nullptr) {
            QMessageBox::warning(this,
                                 "Tabela Vazia",
                                 "Não exitem dados a serem coletados. Mova o mouse sobre a imagem para coletar dados.");
            return;
        }

        QString fileName = QFileDialog::getSaveFileName(this, "Salvar Dados");
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "Erro", "Não foi possível salvar o arquivo.");
            return;
        }

        QTextStream out(&file);
        for (int i = 0; i < pixelDataTable->rowCount(); ++i) {
            out << pixelDataTable->item(i, 2)->text() << "\t"
                << pixelDataTable->item(i, 3)->text() << "\t"
                << pixelDataTable->item(i, 4)->text() << "\t"
                << pixelDataTable->item(i, 5)->text() << "\n";
        }
        file.close();

        QMessageBox::information(this, "Sucesso", "Arquivo Salvo com Sucesso");
    }

    void RockImageUI::cleanTable() {
        auto *pixelDataTable = (PixelDataTable*) ui->dataTablesTab->currentWidget();
        if (pixelDataTable == nullptr) {
            QMessageBox::warning(this,
                                 "Tabela Vazia",
                                 "Não exitem dados a serem coletados. Mova o mouse sobre a imagem para coletar dados.");
            return;
        }

        QMessageBox::StandardButton result = QMessageBox::question(this, "Limpar Tabela", "Tem certeza que deseja limpar os dados coletados na tabela?");
        if (result == QMessageBox::No) {
            return;
        }

        pixelDataTable->clearContents();
        pixelDataTable->setRowCount(0);
    }

    void RockImageUI::applyBinarization() {
        qDebug("Apply Binarization");
    }

    void RockImageUI::loadImage(const QString& filePath) {
        auto *listItem = new QListWidgetItem();

        QString fileName = filePath.section("/", -1, -1);
        QList<QListWidgetItem *> foundItems = ui->imagesList->findItems(fileName, Qt::MatchExactly);

        if (foundItems.empty()) {
            listItem = new QListWidgetItem();
            listItem->setText(fileName);
            listItem->setToolTip(filePath);
            ui->imagesList->addItem(listItem);
        } else {
            listItem = foundItems[0];
        }

        showImage(listItem);
    }

    void RockImageUI::showImage(QListWidgetItem *listWidgetItem) {
        QString filePath = listWidgetItem->toolTip();
        QString fileName = listWidgetItem->text();


        int pixelDataTableIndex = getPixelDataTableByName(fileName);

        if (pixelDataTableIndex == -1) {
            auto *pixelDataTable = new PixelDataTable();
            ui->dataTablesTab->addTab(pixelDataTable, fileName);
            pixelDataTableIndex = ui->dataTablesTab->count();
        }

        ui->dataTablesTab->setCurrentIndex(pixelDataTableIndex);

        auto *subWindow = getSubWidowByName(fileName);

        if (subWindow == nullptr) {
            subWindow = new ImageDisplaySubWindow(filePath, fileName);
            subWindow->setAttribute(Qt::WA_DeleteOnClose);
            ui->openImagesArea->addSubWindow(subWindow);
        }

        subWindow->loadImage(filePath);
        subWindow->show();
    }

    void RockImageUI::collectDataFromImage() {
        auto *activeSubWindow = (ImageDisplaySubWindow*) ui->openImagesArea->currentSubWindow();
        if (activeSubWindow == nullptr) {
            QMessageBox::warning(this,
                                 "Área de Trabalho Vazia",
                                 "Não exitem dados a serem coletados. Abra uma imagem para prosseguir.");
            return;
        }

        auto *imageDisplayWidget = activeSubWindow->getImageLabel();
        if (imageDisplayWidget == nullptr) {
            QMessageBox::warning(this,
                                 "Área de Trabalho Vazia",
                                 "Não exitem dados a serem coletados. Abra uma imagem para prosseguir.");
            return;
        }

        auto *pixelDataTable = (PixelDataTable*) ui->dataTablesTab->currentWidget();
        if (pixelDataTable == nullptr) {
            QMessageBox::warning(this,
                                 "Tabela Vazia",
                                 "Não exitem dados a serem coletados. Mova o mouse sobre a imagem para coletar dados.");
            return;
        }

        QHash<QPoint, QRgb> pixelDataMap = imageDisplayWidget->getPixelDataMap();
        for(auto i = pixelDataMap.constBegin(); i != pixelDataMap.constEnd(); ++i) {
            pixelDataTable->addData(i.key(), i.value(), QString::number(labelData));
        }

        imageDisplayWidget->clearPixelDataMap();
    }

    int RockImageUI::getPixelDataTableByName(const QString& name) {
        for (int i = 0; i < ui->dataTablesTab->count(); ++i) {
            if (ui->dataTablesTab->tabText(i) == name) {
                return i;
            }
        }

        return -1;
    }

    ImageDisplaySubWindow *RockImageUI::getSubWidowByName(const QString& name) {
        for (auto &subWindow : ui->openImagesArea->subWindowList()) {
            if (subWindow->windowTitle() == name) {
                return (ImageDisplaySubWindow*) subWindow;
            }
        }

        return nullptr;
    }

    void RockImageUI::createToolBar() {
        ui->toolBar->clear();
        ui->toolBar->addAction(ui->openImageAction);
        ui->toolBar->addSeparator();
        ui->toolBar->addAction(ui->collectDataAction);
        ui->toolBar->addAction(ui->changeLabelAction);
        ui->toolBar->addAction(ui->cleanTableAction);
        ui->toolBar->addSeparator();
        ui->toolBar->addAction(ui->zoomInAction);
        ui->toolBar->addAction(ui->zoomOutAction);
        ui->toolBar->addSeparator();
        ui->toolBar->addAction(ui->closeAllAction);
    }

    void RockImageUI::setActionsIcons() {
        ui->openImageAction->setIcon(QIcon("../src/assets/icons/add.svg"));
        ui->saveDataAction->setIcon(QIcon("../src/assets/icons/save.svg"));
        ui->cleanTableAction->setIcon(QIcon("../src/assets/icons/clean-table.svg"));
        ui->exitAction->setIcon(QIcon("../src/assets/icons/exit.svg"));
        ui->applyBinarizationAction->setIcon(QIcon("../src/assets/icons/binary.svg"));
        ui->collectDataAction->setIcon(QIcon("../src/assets/icons/collect.svg"));
        ui->changeLabelAction->setIcon(QIcon("../src/assets/icons/change.svg"));
        ui->zoomInAction->setIcon(QIcon("../src/assets/icons/zoom-in.svg"));
        ui->zoomOutAction->setIcon(QIcon("../src/assets/icons/zoom-out.svg"));
        ui->closeAllAction->setIcon(QIcon("../src/assets/icons/close-all.svg"));
    }

    void RockImageUI::closeAllWindows() {
        ui->openImagesArea->closeAllSubWindows();
    }

    void RockImageUI::zoomIn() {
        qDebug("Zoom In");
    }

    void RockImageUI::zoomOut() {
        qDebug("Zoom Out");
    }

    void RockImageUI::changeTargetLabel() {
        if (ui->changeLabelAction->isChecked()) {
            labelData = LabelData::SOLID;
        } else {
            labelData = LabelData::PORE;
        }

        qDebug() << "Label: " << labelData;
    }

} // RockImageUI
