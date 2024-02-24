#include "dialogGetPathToFile.h"
#include "ui_dialogGetPathToFile.h"

DialogGetPathToFile::DialogGetPathToFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGetPathToFile)
{
    ui->setupUi(this);
}

DialogGetPathToFile::~DialogGetPathToFile()
{
    qDebug() << "deleted";
    delete ui;
}


QString DialogGetPathToFile::getPathToFile(QTreeWidgetItem* item, QWidget* parent,
                                           SelectExtensions selectExtensions) {

    QString path;

    if (item == nullptr) {
        path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Wizard Studio projects/";
    }
    else {
        path = item->data(0, Qt::UserRole).toString() + "/";
    }

    DialogGetPathToFile* dialogGetPathToFile = new DialogGetPathToFile(parent);

    if (selectExtensions == SelectExtensions::Disabled) {
        dialogGetPathToFile->ui->comboBoxExtensions->setDisabled(true);
    }

    // if pressed cancel
    if (dialogGetPathToFile->exec() != QDialog::Accepted) {

        delete dialogGetPathToFile;

        return "";
    }

    QString name = dialogGetPathToFile->ui->lineEdit->text();

    if (name.contains('\\') || name.contains('/') || name.contains(':') || name.contains('*') || name.contains('?') ||
        name.contains('<') || name.contains('>') || name.contains('|') || name.contains('"')) {

        QMessageBox::critical(dialogGetPathToFile, "Warning", "name cannot have \\, /, :, *, ?, <, >, |, \"");

        delete dialogGetPathToFile;

        return "";
    }


    QString nameFile = dialogGetPathToFile->ui->lineEdit->text() +
            dialogGetPathToFile->ui->comboBoxExtensions->currentText();

    if (!QFileInfo(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Wizard Studio projects").exists()) {

        if (!QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Wizard Studio projects")) {
            qDebug() << "failed creating folder for projects and files";
        }
    }

    delete dialogGetPathToFile;

    return path + nameFile;
}
