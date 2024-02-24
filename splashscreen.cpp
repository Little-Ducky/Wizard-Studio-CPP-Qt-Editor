#include "splashscreen.h"
#include "ui_splashscreen.h"

SplashScreen::SplashScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);

    listFact.append("First programmer was Ada Lovelace");
    listFact.append("First computer was created in 1945");
    listFact.append("Windows >= Linux");

    ui->labelFact->setText(listFact.at(QRandomGenerator::global()->bounded(listFact.size())));

    setStyleSheet("QWidget {"
                  "   background: #0E0E0E url(':/sources/img/logo.svg') no-repeat center; "
                  "   }"
                  "QLabel { "
                  "   font-weight: bold; "
                  "   font-size: 14px; "
                  "   color: #E6E6E6; "
                  "   font-family: 'Consolas'; "
                  "   margin-bottom: 12px; "
                  "}");
}

SplashScreen::~SplashScreen()
{
    delete ui;
}
