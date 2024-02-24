#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QStringList>
#include <QRandomGenerator>
#include <QTimer>

namespace Ui {
class SplashScreen;
}

class SplashScreen : public QWidget
{
public:
    explicit SplashScreen(QWidget *parent = nullptr);
    ~SplashScreen();

private:
    Ui::SplashScreen *ui;

    QStringList listFact;
};

#endif // SPLASHSCREEN_H
