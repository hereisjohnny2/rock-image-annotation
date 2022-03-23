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

        // File Menu Actions
        connect(ui->openImageAction, SIGNAL(triggered()), this, SLOT(openImage()));
        connect(ui->saveDataAction, SIGNAL(triggered()), this, SLOT(saveTableData()));
        connect(ui->cleanTableAction, SIGNAL(triggered()), this, SLOT(cleanTable()));
        connect(ui->exitAction, &QAction::triggered, [this](){QApplication::quit();});

        // ImageList Events
        ui->imagesList->installEventFilter(this);
        connect(ui->imagesList,
                SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this,
                SLOT(showImage(QListWidgetItem*)));

        // Show Dock Widgets Menu
        showImagesAction = ui->imagesListDockWidget->toggleViewAction();
        showImagesAction->setText("Lista de Imagens");

        showDataTablesAction = ui->tableTabDockWidget->toggleViewAction();
        showDataTablesAction->setText("Tabelas de Dados");

        ui->showDockMenu->addAction(showImagesAction);
        ui->showDockMenu->addAction(showDataTablesAction);

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
        auto pixelDataTable = getCurrentDataTable();
        if (pixelDataTable == nullptr) {
            QMessageBox::warning(this,
                                 "Tabela Vazia",
                                 "Não exitem dados a serem coletados.");
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
        auto pixelDataTable = getCurrentDataTable();
        if (pixelDataTable == nullptr) {
            QMessageBox::warning(this,
                                 "Tabela Vazia",
                                 "Não exitem dados a serem coletados.");
            return;
        }

        QMessageBox::StandardButton result = QMessageBox::question(
                this,
                "Limpar Tabela",
                "Tem certeza que deseja limpar os dados coletados na tabela?");
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
        QListWidgetItem *listItem;

        QString fileName = filePath.section("/", -1, -1);
        QList<QListWidgetItem*> foundItems = ui->imagesList->findItems(fileName, Qt::MatchExactly);

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
        auto *imageDisplayWidget = getCurrentSubWindowImage();
        if (imageDisplayWidget == nullptr) {
            return;
        }

        auto *pixelDataTable = getCurrentDataTable();
        if (pixelDataTable == nullptr) {
            QMessageBox::warning(this,
                                 "Tabela Vazia",
                                 "Não exite tabela com dados a serem coletados.");
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

    void RockImageUI::closeAllWindows() {
        ui->openImagesArea->closeAllSubWindows();
    }

    void RockImageUI::zoomIn() {
        auto *activeSubWindow = (ImageDisplaySubWindow*) ui->openImagesArea->currentSubWindow();
        if (activeSubWindow == nullptr) {
            QMessageBox::warning(this,
                                 "Área de Trabalho Vazia",
                                 "Não existe nenhuma janela ativa no momento.");
            return;
        }

        activeSubWindow->scaleImage(1.25);
    }

    void RockImageUI::zoomOut() {
        auto currentSubWindow = getCurrentSubWindow();
        if (currentSubWindow == nullptr) {
            QMessageBox::warning(this,
                                 "Área de Trabalho Vazia",
                                 "Não existe nenhuma janela ativa no momento.");
            return;
        }
        currentSubWindow->scaleImage(0.75);
    }

    void RockImageUI::changeTargetLabel() {
        if (ui->changeLabelAction->isChecked()) {
            labelData = LabelData::SOLID;
        } else {
            labelData = LabelData::PORE;
        }

        qDebug() << "Label: " << labelData;
    }

    ImageDisplaySubWindow *RockImageUI::getCurrentSubWindow() {
        auto *activeSubWindow = dynamic_cast<ImageDisplaySubWindow*>(ui->openImagesArea->currentSubWindow());
        return activeSubWindow;
    }

    PixelDataTable *RockImageUI::getCurrentDataTable() {
        auto *pixelDataTable = dynamic_cast<PixelDataTable*>(ui->dataTablesTab->currentWidget());
        return pixelDataTable;
    }

    ImageDisplayWidget *RockImageUI::getCurrentSubWindowImage() {
        auto currentSubWindow = getCurrentSubWindow();
        if (currentSubWindow == nullptr) {
            QMessageBox::warning(this,
                                 "Área de Trabalho Vazia",
                                 "Não existe nenhuma janela ativa no momento com dados a serem coletados.");
            return nullptr;
        }

        auto *imageDisplayWidget = dynamic_cast<ImageDisplayWidget*>(currentSubWindow->getImageLabel());
        return imageDisplayWidget;
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

    bool RockImageUI::eventFilter(QObject *obj, QEvent *event) {
        if (event->type() == QEvent::KeyPress) {
            auto *keyEvent = dynamic_cast<QKeyEvent *>(event);
            if(keyEvent->key() == 16777220) {
                showImage(ui->imagesList->currentItem());
                return true;
            }

            if(keyEvent->key() == 16777223) {
                int index = ui->imagesList->currentRow();
                QString name = ui->imagesList->currentItem()->text();
                deleteImage(name);
                ui->imagesList->takeItem(index);
                return true;
            }

            return false;
        } else {
            return QObject::eventFilter(obj, event);
        }
    }

    void RockImageUI::deleteImage(const QString& name) {
        auto subWindow = getSubWidowByName(name);
        if (subWindow != nullptr) {
            subWindow->close();
        }

        auto dataTableIndex = getPixelDataTableByName(name);
        if (dataTableIndex > -1) {
            ui->dataTablesTab->removeTab(dataTableIndex);
        }
    }
} // RockImageUI
