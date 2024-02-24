#ifndef FORMOPTIONVIEW_H
#define FORMOPTIONVIEW_H

#include <QWidget>

namespace Ui {
class FormOptionView;
}

class FormOptionView : public QWidget
{
    Q_OBJECT

    friend class DialogSettingProgram;

public:
    explicit FormOptionView(QWidget *parent = nullptr);
    ~FormOptionView();

private:
    Ui::FormOptionView *ui;
};

#endif // FORMOPTIONVIEW_H
