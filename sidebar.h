#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QTreeWidget>
#include <QHeaderView>
#include <QDir>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QMessageBox>
#include <QProcess>
#include <QInputDialog>
#include <QMenu>

#include "dialogGetPathToFile.h"
#include "dialogunacceptable.h"
#include "dialogpropertiesproject.h"
#include "dialogGetPathToFile.h"

class SideBar : public QTreeWidget
{
    Q_OBJECT

public:
    explicit SideBar(QWidget *parent = nullptr);
    ~SideBar();

public:
    // menu handels
    void setSelectedProject(QTreeWidgetItem*);
    QTreeWidgetItem* getSelectedProject();

    void openFile(QTreeWidgetItem*);

    void addNewFolder(QTreeWidgetItem*);
    void addNewFile(QTreeWidgetItem*, DialogGetPathToFile::SelectExtensions =
                                      DialogGetPathToFile::SelectExtensions::Actived);

    void renameItem(QTreeWidgetItem*);
    void deleteItem(QTreeWidgetItem*);
    void removeItem(QTreeWidgetItem*, bool isSaveFile = true);

private slots:
    void showMenu(const QPoint&);

public slots:
    void provide(const QString&, QTreeWidgetItem* = nullptr);
    
public:
    void removeFromSidebar(QList<QTreeWidgetItem*>&, QStringList&);
    void renameInSidebar(QList<QTreeWidgetItem*>&, const QString&, const QString&);
    bool contains(const QString&);
    bool isValidName(const QString&);

private:
    void checkItemAndFillLists(const QString&);
    void addToSideBar(const QString&, QTreeWidgetItem*);
    QTreeWidgetItem* getItemByPath(const QString&);
    void getInsertedItems(QStringList&, QTreeWidgetItem*);

    void dragEnterEvent(QDragEnterEvent*) override;
    void dragLeaveEvent(QDragLeaveEvent*) override;
    void dragMoveEvent(QDragMoveEvent*) override;
    void dropEvent(QDropEvent*) override;

    void createCMakeLists(QTreeWidgetItem*);

signals:
    void switchToWorkSectionSignal();
    void itemClickedSignal(QTreeWidgetItem*, int);

    void renameItemSignal(const QStringList&, const QStringList&);
    void removeItemSignal(const QStringList&, bool);

private:
    QStringList invalidFileExtensionList;
    QStringList duplicateList;

    QStringList allowedExtension;

    QTreeWidgetItem* selectedProject;
};

#endif // SIDEBAR_H
