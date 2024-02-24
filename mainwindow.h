#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QProcess>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QInputDialog>

#include "splashscreen.h"
#include "dialogsettingprogram.h"
#include "dialogGetPathToFile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupStyle(const QString&);

    void closeEvent(QCloseEvent*);

private:
    Ui::MainWindow *ui;

    QString styleSheetString;

    QPushButton* statusBarText;

    DialogSettingProgram* dialogProgramSetting;

    void runCMake();
    void runCompiledExe();

    QProcess* cmakeRunConfigProcess;
    QProcess* cmakeRunBuildProcess;

    bool mustRunExe;
    bool hasError;

signals:
    void runCMakeMakeBuildSignal();
};
#endif // MAINWINDOW_H
