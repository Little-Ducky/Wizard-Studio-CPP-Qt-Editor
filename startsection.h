#ifndef STARTSECTION_H
#define STARTSECTION_H

#include <QWidget>
#include <QDir>
#include <QMimeData>
#include <QDragEnterEvent>

namespace Ui {
class StartSection;
}

class StartSection : public QWidget
{
    Q_OBJECT

public:
    explicit StartSection(QWidget *parent = nullptr);
    ~StartSection();

    void dragEnterEvent(QDragEnterEvent*) override;
    void dragLeaveEvent(QDragLeaveEvent*) override;
    void dragMoveEvent(QDragMoveEvent*) override;
    void dropEvent(QDropEvent*) override;

private:
    Ui::StartSection *ui;

signals:
    void provideToSidebarSignal(const QString&);
};

#endif // STARTSECTION_H
