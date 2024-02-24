#include "formoptionview.h"
#include "ui_formoptionview.h"

FormOptionView::FormOptionView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormOptionView)
{
    ui->setupUi(this);
}

FormOptionView::~FormOptionView()
{
    delete ui;
}
