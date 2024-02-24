#ifndef DIALOGSETTINGPROGRAM_H
#define DIALOGSETTINGPROGRAM_H

#include <QDialog>
#include <QCloseEvent>
#include <QPushButton>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

#include "ui_formoptionbuild.h"
#include "ui_formoptionview.h"

namespace Ui {
class DialogSettingProgram;
}

class DialogSettingProgram : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettingProgram(QWidget *parent = nullptr);
    ~DialogSettingProgram();

    QString getTheme() const;
    QString getFont() const;

    QString getSelectedCompiler();
    QString getSelectedTypeBuild();

private:
    Ui::DialogSettingProgram *ui;

    QJsonObject jsonObject;

    void setupJson();
    void updateJson(const QString&, const QString&);
    void closeEvent(QCloseEvent*);

    void cancelChangeOptions();
signals:

    void changeThemeSignal(const QString&);
    void changeFontSignal(const QString&);
};

#endif // DIALOGSETTINGPROGRAM_H
