#ifndef FORMOPTIONBUILD_H
#define FORMOPTIONBUILD_H

#include <QWidget>

namespace Ui {
class FormOptionBuild;
}

class FormOptionBuild : public QWidget
{
    Q_OBJECT

    friend class DialogSettingProgram;

public:
    explicit FormOptionBuild(QWidget *parent = nullptr);
    ~FormOptionBuild();

private:
    Ui::FormOptionBuild *ui;
};

#endif // FORMOPTIONBUILD_H
