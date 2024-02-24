
#include <QApplication>

#include "mainwindow.h"
#include "splashscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    SplashScreen s;
    s.show();

    QTimer::singleShot(3300, [&](){
        s.hide();
        w.show();
    });

    return a.exec();
}

