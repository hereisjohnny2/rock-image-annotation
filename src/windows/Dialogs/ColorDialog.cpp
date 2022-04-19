#include <QLabel>
#include <QDialogButtonBox>
#include "ColorDialog.h"

ColorDialog::ColorDialog(QWidget *parent) {
    lytMain = new QFormLayout(this);

    createColorInput("Vermelho");
    createColorInput("Verde");
    createColorInput("Azul");

    auto colorBox = new QWidget();


    auto *buttonBox = new QDialogButtonBox
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lytMain->addWidget(buttonBox);

    bool conn = connect(buttonBox, &QDialogButtonBox::accepted,
                        this, &ColorDialog::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox, &QDialogButtonBox::rejected,
                   this, &ColorDialog::reject);
    Q_ASSERT(conn);

    setLayout(lytMain);
}

QList<int> ColorDialog::getStrings(QWidget *parent, bool *ok)
{
    auto *dialog = new ColorDialog(parent);

    QList<int> list;
    const int ret = dialog->exec();
    if (ok)
        *ok = !!ret;

    if (ret) {
        foreach (auto field, dialog->fields) {
            list << field->value();
        }
    }

    dialog->deleteLater();
    return list;
}


void ColorDialog::createColorInput(const QString &label) {
    auto *tLabel = new QLabel(label, this);
    auto *value = new QSpinBox(this);
    value->setMinimum(0);
    value->setMaximum(255);
    value->setValue(125);
    lytMain->addRow(tLabel, value);

    fields << value;
}
