#ifndef DIALOGUNACCEPTABLE_H
#define DIALOGUNACCEPTABLE_H

#include <QDialog>

namespace Ui {
class DialogUnacceptable;
}

class DialogUnacceptable : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUnacceptable(const QString&, QWidget *parent = nullptr);
    ~DialogUnacceptable();

private:
    Ui::DialogUnacceptable *ui;
};

#endif // DIALOGUNACCEPTABLE_H
