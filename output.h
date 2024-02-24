#ifndef OUTPUT_H
#define OUTPUT_H

#include <QWidget>
#include <QProcess>
#include <QScrollBar>
#include <QThread>

namespace Ui {
class Output;
}

class Output : public QWidget
{
    Q_OBJECT

public:
    explicit Output(QWidget *parent = nullptr);
    ~Output();

    void setDataOutput(const QString&);
    QString getDataOutput();

    void setBottomVerticalScrollBar();

    void clearFieldOutput();
private:
    Ui::Output *ui;
};

#endif // OUTPUT_H
