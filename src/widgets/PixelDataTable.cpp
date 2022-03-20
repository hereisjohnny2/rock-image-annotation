//
// Created by joao on 19/03/2022.
//

#include "PixelDataTable.h"

PixelDataTable::PixelDataTable() {
    this->setTableHeaders();

    QSizePolicy updatedSizePolicy;
    updatedSizePolicy.setHorizontalStretch(0);
    updatedSizePolicy.setVerticalStretch(0);
    updatedSizePolicy.setHeightForWidth(sizePolicy().hasHeightForWidth());

    this->setSizePolicy(updatedSizePolicy);
}

void PixelDataTable::setTableHeaders() {
    this->setColumnCount(6);
    this->setHorizontalHeaderItem(0, new QTableWidgetItem("PosX"));
    this->setHorizontalHeaderItem(1, new QTableWidgetItem("PosY"));
    this->setHorizontalHeaderItem(2, new QTableWidgetItem("Vermelho"));
    this->setHorizontalHeaderItem(3, new QTableWidgetItem("Verde"));
    this->setHorizontalHeaderItem(4, new QTableWidgetItem("Azul"));
    this->setHorizontalHeaderItem(5, new QTableWidgetItem("Label"));
}

void PixelDataTable::addData(const QPoint &point, const QVector<int>& rgba, const QString& label) {
    this->setRowCount(this->rowCount() + 1);

    this->addCell(0, QString::number(point.x()));
    this->addCell(1, QString::number(point.y()));
    this->addCell(2, QString::number(rgba[0]));
    this->addCell(3, QString::number(rgba[1]));
    this->addCell(4, QString::number(rgba[2]));
    this->addCell(5, label);
}

void PixelDataTable::addCell(int column, const QString& value) {
    setItem(this->rowCount(), column, new QTableWidgetItem(value));
}
