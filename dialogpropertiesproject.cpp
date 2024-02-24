#include "dialogpropertiesproject.h"
#include "ui_dialogpropertiesproject.h"
#include "ui_dialogfield.h"

DialogPropertiesProject::DialogPropertiesProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPropertiesProject)
{
    ui->setupUi(this);

    fieldAddLibraries = new DialogField(this);
    fieldAddLibraries->setWindowTitle("Add libraries");
    fieldAddIncludes = new DialogField(this);
    fieldAddIncludes->setWindowTitle("Add includes");
    fieldAddInputes = new DialogField(this);
    fieldAddInputes->setWindowTitle("Add inputes");

    connect(ui->pushButtonAddInclude, &QPushButton::clicked, [=] {
        fieldAddIncludes->show();
    });

    connect(ui->pushButtonAddInput, &QPushButton::clicked, [=] {
        fieldAddInputes->show();
    });

    connect(ui->pushButtonAddLibrary, &QPushButton::clicked, [=] {
        fieldAddLibraries->show();
    });

}

DialogPropertiesProject::~DialogPropertiesProject()
{
    delete ui;
}

QString DialogPropertiesProject::getAdditionalLibraries() const {
    return fieldAddLibraries->ui->field->toPlainText();
}
QString DialogPropertiesProject::getAdditionalIncludes() const {
    return fieldAddIncludes->ui->field->toPlainText();
}
