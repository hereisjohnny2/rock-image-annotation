#include "rockimageui.h"
#include "ui_rockimageui.h"
#include <QtWidgets>
#include <map>
#include <QDialog>
#include "../Dialogs/CustomMessageDialogs.h"
#include "../Dialogs/ColorDialog.h"

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

        // Images Menu Actions
        connect(ui->addLayerAction, SIGNAL(triggered()), this, SLOT(addLayer()));
        connect(ui->removeLayerAction, SIGNAL(triggered()), this, SLOT(removeCurrentLayerLayer()));
        connect(ui->increaseWidthAction, SIGNAL(triggered()), this, SLOT(increaseWidth()));
        connect(ui->decreaseWidthAction, SIGNAL(triggered()), this, SLOT(decreaseWidth()));
        connect(ui->chooseColorAction, SIGNAL(triggered()), this, SLOT(chooseColor()));

        // ImageList Events
        ui->imagesList->installEventFilter(this);
        connect(ui->imagesList,
                SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this,
                SLOT(showImage(QListWidgetItem*)));

        // ImageTree Events
        ui->imageTree->installEventFilter(this);
        ui->imageTree->installEventFilter(this);
        connect(ui->imageTree,
                SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
                this,
                SLOT(showLayer(QTreeWidgetItem*,int)));

        // Show Dock Widgets Menu
        ui->tableTabDockWidget->setVisible(false);
        ui->imagesListDockWidget->setVisible(false);

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
        QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir Imagem"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
        QFile file(fileName);

        if (!file.exists()) {
            QMessageBox::warning(this, "Error", "Não foi possível abrir a imagem selecionada.");
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

        QString filter("Text files (*.txt, *.dat)");
        QString fileName = QFileDialog::getSaveFileName(
                this, tr("Salvar Dados"), QDir::homePath(), filter, &filter);
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

        bool result = CustomMessageDialogs::Question(
                this,
                "Limpar Tabela",
                "Tem certeza que deseja limpar os dados coletados na tabela?");
        if (!result) {
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

        auto foundTreeItems = ui->imageTree->findItems(fileName, Qt::MatchExactly);
        if (foundTreeItems.isEmpty()) {
            auto treeItem = new QTreeWidgetItem();
            treeItem->setText(0, fileName);
            ui->imageTree->addTopLevelItem(treeItem);
        }

        ui->imagesListDockWidget->setVisible(true);
        ui->tableTabDockWidget->setVisible(true);

        showImage(listItem);
    }

    void RockImageUI::showImage(QListWidgetItem *listWidgetItem) {
        QString filePath = listWidgetItem->toolTip();
        QString fileName = listWidgetItem->text();

        auto pixelDataTableIndex = getPixelDataIndexTableByName(fileName);
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
        auto window = getCurrentSubWindow();
        if (window == nullptr) {
            return;
        }

        auto *imageDisplayWidget = window->getTopLayerImage();
        if (imageDisplayWidget == nullptr) {
            return;
        }

        auto pixelDataTable = getPixelDataTableByName(window->windowTitle());
        if (pixelDataTable == nullptr) {
            QMessageBox::warning(this,
                                 "Tabela Vazia",
                                 "Não exite tabela com dados a serem coletados.");
            return;
        }

        ui->dataTablesTab->setCurrentWidget(pixelDataTable);

        QHash<QPoint, QRgb> pixelDataMap = imageDisplayWidget->getPixelDataMap();
        for(auto i = pixelDataMap.constBegin(); i != pixelDataMap.constEnd(); ++i) {
            pixelDataTable->addData(i.key(), i.value(), imageDisplayWidget->getLabel());
        }

        imageDisplayWidget->clearPixelDataMap();
    }

    PixelDataTable* RockImageUI::getPixelDataTableByName(const QString& name) {
        for (int i = 0; i < ui->dataTablesTab->count(); ++i) {
            if (ui->dataTablesTab->tabText(i) == name) {
                return dynamic_cast<PixelDataTable *>(ui->dataTablesTab->widget(i));
            }
        }

        return nullptr;
    }

    ImageDisplaySubWindow *RockImageUI::getSubWidowByName(const QString& name) {
        for (auto &subWindow : ui->openImagesArea->subWindowList()) {
            if (subWindow->windowTitle() == name) {
                return dynamic_cast<ImageDisplaySubWindow *>(subWindow);
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

    ImageDisplayWidget *RockImageUI::getCurrentSubWindowTopLayerImage() {
        auto currentSubWindow = getCurrentSubWindow();
        if (currentSubWindow == nullptr) {
            QMessageBox::warning(this,
                                 "Área de Trabalho Vazia",
                                 "Não existe nenhuma janela ativa no momento com dados a serem coletados.");
            return nullptr;
        }

        auto *imageDisplayWidget = dynamic_cast<ImageDisplayWidget*>(currentSubWindow->getTopLayerImage());
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
        ui->openImageAction->setIcon(QIcon("../assets/icons/add.svg"));
        ui->saveDataAction->setIcon(QIcon("../assets/icons/save.svg"));
        ui->cleanTableAction->setIcon(QIcon("../assets/icons/clean-table.svg"));
        ui->exitAction->setIcon(QIcon("../assets/icons/exit.svg"));
        ui->applyBinarizationAction->setIcon(QIcon("../assets/icons/binary.svg"));
        ui->collectDataAction->setIcon(QIcon("../assets/icons/collect.svg"));
        ui->changeLabelAction->setIcon(QIcon("../assets/icons/change.svg"));
        ui->zoomInAction->setIcon(QIcon("../assets/icons/zoom-in.svg"));
        ui->zoomOutAction->setIcon(QIcon("../assets/icons/zoom-out.svg"));
        ui->closeAllAction->setIcon(QIcon("../assets/icons/close-all.svg"));
        ui->addLayerAction->setIcon(QIcon("../assets/icons/layer.svg"));
        ui->removeLayerAction->setIcon(QIcon("../assets/icons/remove.svg"));
    }

    bool RockImageUI::eventFilter(QObject *obj, QEvent *event) {
        if (event->type() == QEvent::KeyPress) {
            auto *keyEvent = dynamic_cast<QKeyEvent *>(event);

            if(keyEvent->key() == ENTER_KEY_CODE) {
                if (dynamic_cast<QListWidget*>(obj) != nullptr) {
                    showImage(ui->imagesList->currentItem());
                    return true;
                }

                if (dynamic_cast<QTreeWidget*>(obj) != nullptr) {
                    showLayer(ui->imageTree->currentItem(), 0);
                    return true;
                }
            }

            if(keyEvent->key() == DELETE_KEY_CODE) {
                if (dynamic_cast<QListWidget*>(obj) != nullptr) {
                    deleteCurrentImage();
                    return true;
                }

                if (dynamic_cast<QTreeWidget*>(obj) != nullptr) {
                    auto item = ui->imageTree->currentItem();
                    if (item->parent() == nullptr) return false;

                    return removeLayer(item);
                }
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

        auto dataTableIndex = getPixelDataIndexTableByName(name);
        if (dataTableIndex > -1) {
            ui->dataTablesTab->removeTab(dataTableIndex);
        }
    }

    void RockImageUI::deleteCurrentImage() {
        bool  result = CustomMessageDialogs::Question(
                this,
                "Remover Imagem",
                "Tem certeza que deseja remover essa imagem da área da trabalho?");

        if (!result) {
            return;
        }

        int index = ui->imagesList->currentRow();
        QString name = ui->imagesList->currentItem()->text();
        deleteImage(name);
        ui->imagesList->takeItem(index);

        ui->imageTree->takeTopLevelItem(index);

        if (ui->imagesList->currentRow() == -1) {
            ui->tableTabDockWidget->setVisible(false);
            ui->imagesListDockWidget->setVisible(false);
        }
    }

    void RockImageUI::addLayer() {
        auto window = getCurrentSubWindow();
        if (window == nullptr) return;

        bool isOk;
        QString label = QInputDialog::getText(this,
                                              tr("Adicionar Camada"),
                                              tr("Label:"), QLineEdit::Normal, "layer", &isOk);
        if (!isOk or label.isEmpty()) {
            QMessageBox::warning(this, tr("Adicionar Camada"), tr("Toda camada deve possuir uma label."));
            return;
        }

        window->addNewLayer(label);

        auto layerTreeItem = new QTreeWidgetItem();
        layerTreeItem->setText(0, label);
        layerTreeItem->setBackground(1, window->getTopLayerImage()->getPenBrush());
        auto node = ui->imageTree->findItems(window->windowTitle(), Qt::MatchExactly)[0];
        node->addChild(layerTreeItem);

        ui->imageTree->setCurrentItem(layerTreeItem);
    }

    void RockImageUI::removeCurrentLayerLayer() {
        auto window = getCurrentSubWindow();
        if (window == nullptr) return;
        window->removeCurrentLayer();
    }

    int RockImageUI::getPixelDataIndexTableByName(const QString &name) {
        auto table = getPixelDataTableByName(name);
        if (table == nullptr) return -1;
        return ui->dataTablesTab->indexOf(table);
    }

    void RockImageUI::showLayer(QTreeWidgetItem *treeWidgetItem, int column) {
        QString subWindowName, name;

        if (treeWidgetItem->parent() == nullptr) {
            subWindowName = treeWidgetItem->text(0);
            name = StackedImagesWidget::BASE_IMAGE;
        } else {
            subWindowName = treeWidgetItem->parent()->text(0);
            name = treeWidgetItem->text(column);
        }

        auto subWindow = getSubWidowByName(subWindowName);
        subWindow->showLayer(name);
    }

    bool RockImageUI::removeLayer(QTreeWidgetItem* item) {
        bool  result = CustomMessageDialogs::Question(
                this,
                "Remover Camada",
                "Tem certeza que deseja remover essa camada?");
        if (!result) return false;

        auto window = getCurrentSubWindow();
        if (window == nullptr) return false;

        auto parent = item->parent();
        window->removeLayerByName(item->text(0));
        parent->removeChild(item);
        return true;
    }

    void RockImageUI::increaseWidth() {
        auto window = getCurrentSubWindow();
        if (window == nullptr) return;

        window->updatePenWidth(1);
    }

    void RockImageUI::decreaseWidth() {
        auto window = getCurrentSubWindow();
        if (window == nullptr) return;

        window->updatePenWidth(-1);
    }

    void RockImageUI::chooseColor() {
        auto window = getCurrentSubWindow();
        if (window == nullptr) return;

        bool isOk;
        QList<int> colors = ColorDialog::getStrings(this, &isOk);
        if (!isOk or colors.isEmpty()) {
            QMessageBox::warning(this, tr("Mudar Color"), tr("Selecionar ao menos um valor"));
            return;
        }

        QColor color(colors[0], colors[1], colors[2]);

        auto node = ui->imageTree->currentItem();
        if (node == nullptr) return;
        node->setBackground(1, color);

        window->updatePenBrush(color);
    }
} // RockImageUI
