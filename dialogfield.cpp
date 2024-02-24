#include "dialogfield.h"
#include "ui_dialogfield.h"

DialogField::DialogField(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogField)
{
    ui->setupUi(this);

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, [&] {

        previousData = ui->field->toPlainText();
    });

    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, [&] {

        ui->field->setPlainText(previousData);
    });
}

void DialogField::closeEvent(QCloseEvent* e) {

    ui->field->setPlainText(previousData);

    e->accept();
}

DialogField::~DialogField()
{
    delete ui;
}
