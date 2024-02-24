#include "output.h"
#include "ui_output.h"

Output::Output(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Output)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
}

void Output::setBottomVerticalScrollBar() {
    ui->outputField->verticalScrollBar()->setValue(ui->outputField->verticalScrollBar()->maximum());
}

void Output::clearFieldOutput() {
    ui->outputField->clear();
}

void Output::setDataOutput(const QString& data) {
    ui->outputField->setPlainText(data);
}

QString Output::getDataOutput() {
    return ui->outputField->toPlainText();
}

Output::~Output()
{
    delete ui;
}
