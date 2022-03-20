#include "PixelDataTable.h"
#include <QHeaderView>
#include <QTableWidgetItem>

PixelDataTable::PixelDataTable() {
    setTableHeaders();

    QSizePolicy updatedSizePolicy;
    updatedSizePolicy.setHorizontalStretch(0);
    updatedSizePolicy.setVerticalStretch(0);
    updatedSizePolicy.setHeightForWidth(sizePolicy().hasHeightForWidth());

    setSizePolicy(updatedSizePolicy);
    horizontalHeader()->setStretchLastSection(true);
}

void PixelDataTable::setTableHeaders() {
    setColumnCount(6);
    setHorizontalHeaderItem(0, new QTableWidgetItem("PosX"));
    setHorizontalHeaderItem(1, new QTableWidgetItem("PosY"));
    setHorizontalHeaderItem(2, new QTableWidgetItem("Vermelho"));
    setHorizontalHeaderItem(3, new QTableWidgetItem("Verde"));
    setHorizontalHeaderItem(4, new QTableWidgetItem("Azul"));
    setHorizontalHeaderItem(5, new QTableWidgetItem("Label"));
}

void PixelDataTable::addData(const QPoint &point, const QRgb& rgb, const QString& label) {
    setRowCount(rowCount() + 1);

    addCell(0, QStringLiteral("%1").arg(point.x()));
    addCell(1, QStringLiteral("%1").arg(point.y()));
    addCell(2, QStringLiteral("%1").arg(qRed(rgb)));
    addCell(3, QStringLiteral("%1").arg(qGreen(rgb)));
    addCell(4, QStringLiteral("%1").arg(qBlue(rgb)));
    addCell(5, label);
}

void PixelDataTable::addCell(int column, const QString& value) {
    setItem(rowCount() - 1, column, new QTableWidgetItem(value));
}
