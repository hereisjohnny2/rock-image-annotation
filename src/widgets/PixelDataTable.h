//
// Created by joao on 19/03/2022.
//

#ifndef ROCK_IMAGE_CPP_PIXELDATATABLE_H
#define ROCK_IMAGE_CPP_PIXELDATATABLE_H


#include <QTableWidget>

class PixelDataTable : public QTableWidget {
public:
    PixelDataTable();
    void addData(const QPoint& point, const QRgb& rgb, const QString& label);

private:
    void setTableHeaders();
    void addCell(int column, const QString& value);
};


#endif //ROCK_IMAGE_CPP_PIXELDATATABLE_H
