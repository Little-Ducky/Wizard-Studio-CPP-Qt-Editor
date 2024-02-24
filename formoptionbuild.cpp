#include "formoptionbuild.h"
#include "ui_formoptionbuild.h"

FormOptionBuild::FormOptionBuild(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormOptionBuild)
{
    ui->setupUi(this);
}

FormOptionBuild::~FormOptionBuild()
{
    delete ui;
}
