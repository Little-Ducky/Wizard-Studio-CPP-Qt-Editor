#include "dialogunacceptable.h"
#include "ui_dialogunacceptable.h"

DialogUnacceptable::DialogUnacceptable(const QString& text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUnacceptable)
{
    ui->setupUi(this);
    ui->plainPlainTextEdit->setPlainText(text);
}

DialogUnacceptable::~DialogUnacceptable()
{
    delete ui;
}
