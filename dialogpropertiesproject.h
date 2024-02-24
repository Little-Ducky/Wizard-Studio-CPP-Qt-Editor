#ifndef DIALOGPROPERTIESPROJECT_H
#define DIALOGPROPERTIESPROJECT_H

#include <QDialog>
#include <QCloseEvent>

#include "dialogfield.h"


namespace Ui {
class DialogPropertiesProject;
}

class DialogPropertiesProject : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPropertiesProject(QWidget *parent = nullptr);
    ~DialogPropertiesProject();

    QString getAdditionalLibraries() const;
    QString getAdditionalIncludes() const;

private:
    Ui::DialogPropertiesProject *ui;

    DialogField* fieldAddLibraries;
    DialogField* fieldAddIncludes;
    DialogField* fieldAddInputes;

};

#endif // DIALOGPROPERTIESPROJECT_H
