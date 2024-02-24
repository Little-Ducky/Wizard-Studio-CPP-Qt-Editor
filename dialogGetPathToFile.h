#ifndef DIALOGGETPATHTOFILE_H
#define DIALOGGETPATHTOFILE_H

#include <QDialog>
#include <QCloseEvent>
#include <QPushButton>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QTreeWidgetItem>

namespace Ui {
class DialogGetPathToFile;
}

class DialogGetPathToFile : public QDialog
{
    Q_OBJECT

private:
    explicit DialogGetPathToFile(QWidget *parent = nullptr);
    ~DialogGetPathToFile();

private:
    Ui::DialogGetPathToFile *ui;

public:

    enum SelectExtensions {
        Actived,
        Disabled
    };

    static QString getPathToFile(QTreeWidgetItem*, QWidget* parent,
                                 SelectExtensions selectExtensions = SelectExtensions::Actived);
};

#endif // DIALOGGETPATHTOFILE_H
