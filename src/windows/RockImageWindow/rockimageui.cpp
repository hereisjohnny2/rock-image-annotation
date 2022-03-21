#include "rockimageui.h"
#include "ui_rockimageui.h"
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
        qDebug("Clean Table");
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
            pixelDataTable->addData(i.key(), i.value(), QString("Solid"));
        }
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

} // RockImageUI
