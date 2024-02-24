#ifndef DIALOGFIELD_H
#define DIALOGFIELD_H

#include <QDialog>
#include <QCloseEvent>
#include <QPushButton>

namespace Ui {
class DialogField;
}

class DialogField : public QDialog
{
    Q_OBJECT

    friend class DialogPropertiesProject;

public:
    explicit DialogField(QWidget *parent = nullptr);
    ~DialogField();

private:
    void closeEvent(QCloseEvent*) override;

private:
    Ui::DialogField *ui;

    QString previousData;
};

#endif // DIALOGFIELD_H
