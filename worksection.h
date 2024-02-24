#ifndef WORKSECTION_H
#define WORKSECTION_H

#include <QWidget>
#include <QFileInfo>
#include <QPushButton>
#include <QTabBar>
#include <QMessageBox>
#include <QTreeWidgetItemIterator>
#include <QTreeWidgetItem>

#include "linenumarea.h"

namespace Ui {
class WorkSection;
}

class WorkSection : public QWidget
{
    Q_OBJECT

public:
    explicit WorkSection(QWidget *parent = nullptr);
    ~WorkSection();

    void addToTabWidget(const QString&);
    void removeTabs(const QStringList&, bool = true);
    void renameTabs(const QStringList&, const QStringList&);
    void setCurrentWidget(QWidget*);

    QWidget* getValue(const QString&) const;
    QString getKey(QWidget*) const;
    QStringList getKeys() const;

    QString getCurrentPathToFile() const;
    QString getCurrentTextInTextEdit() const;

    void saveAllOpenedFilesByProject(QTreeWidgetItem*);

private:
    Ui::WorkSection *ui;

    QMap<QString, QWidget*> pathToFileMap;

signals:
    void changedTabSignal();
};

#endif // WORKSECTION_H
