#include "sidebar.h"

SideBar::SideBar(QWidget *parent)
    : QTreeWidget{parent}
{
    //allowed (supported) extensions
    allowedExtension << "cpp" << "h" << "txt" << "hpp" << "cc" << "cxx" <<
                     "inl" << "ui" << "lib" << "dll" << "so" << "a" << "c++";

    selectedProject = nullptr;

    // other style
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);


    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this,  SLOT(showMenu(const QPoint&)));

    connect(this, &QTreeWidget::itemClicked, [=](QTreeWidgetItem *current, int column) {

        emit itemClickedSignal(current, column);
    });
}

SideBar::~SideBar() {}


void SideBar::showMenu(const QPoint& pos) {
    QTreeWidgetItem* item = currentItem();

    if (!item || !item->isSelected()) return;

    QString path = item->data(0, Qt::UserRole).toString();

    QMenu menu;

    QAction selectProjectAction("set as currect project");
    QAction openFileAction("open");
    QAction addNewFolderAction("add new folder");
    QAction addNewFileAction("add new file");
    QAction renameAction("rename");
    QAction removeAction("remove");
    QAction deleteAction("delete");

    selectProjectAction.setIcon(QIcon(":/sources/img/icons/current.svg"));

    openFileAction.setIcon(QIcon(":/sources/img/icons/openFile.svg"));

    addNewFolderAction.setIcon(QIcon(":/sources/img/icons/createProject.svg"));
    addNewFileAction.setIcon(QIcon(":/sources/img/icons/createFile.svg"));

    renameAction.setIcon(QIcon(":/sources/img/icons/rename.svg"));

    removeAction.setIcon(QIcon(":/sources/img/icons/remove.svg"));
    deleteAction.setIcon(QIcon(":/sources/img/icons/delete.svg"));

    if (item->parent() == nullptr && QFileInfo(path).isDir()) {
        menu.addAction(&selectProjectAction);
        menu.addAction(&addNewFolderAction);
        menu.addAction(&addNewFileAction);

    } else if (QFileInfo(path).isDir()) {
        menu.addAction(&addNewFolderAction);
        menu.addAction(&addNewFileAction);
    } else if (!QFileInfo(path).isDir()) {
        menu.addAction(&openFileAction);
    }

    menu.addAction(&renameAction);
    menu.addAction(&removeAction);
    menu.addAction(&deleteAction);

    QAction* selectedAction = menu.exec(mapToGlobal(pos));

    if (selectedAction == &selectProjectAction) {
        setSelectedProject(item);
    }
    else if (selectedAction == &openFileAction) {
        openFile(item);
    }
    else if (selectedAction == &addNewFolderAction) {
        addNewFolder(item);
    }
    else if (selectedAction == &addNewFileAction) {
        addNewFile(item);
    }
    else if (selectedAction == &renameAction) {
        renameItem(item);
    }
    else if (selectedAction == &removeAction) {
        removeItem(item);
    }
    else if (selectedAction == &deleteAction) {
        deleteItem(item);
    }
}


void SideBar::setSelectedProject(QTreeWidgetItem* item) {
    selectedProject = item;
}
QTreeWidgetItem* SideBar::getSelectedProject() {
    return selectedProject;
}

void SideBar::openFile(QTreeWidgetItem* item) {
    emit itemClickedSignal(item, 0);
}

void SideBar::addNewFolder(QTreeWidgetItem* item) {

    bool ok;

    QString name = QInputDialog::getText(this, "Create new project", "Name:", QLineEdit::Normal, "", &ok);

    if (!ok || !isValidName(name)) {

        return;
    }

    QString path = ((item == nullptr) ?
                        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Wizard Studio projects" :
                        item->data(0, Qt::UserRole).toString());

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "Name cannot be empty");

        return;
    }

    if (item != nullptr && !contains(path + "/" + name) && QFileInfo(path + "/" + name).exists() &&
            QMessageBox::question(this, "File exists and it's not part of project",
                                  "Do you wish update your project ?",
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {

        QTreeWidgetItem* parentItem = item;

        while (parentItem->parent()) {

            parentItem = parentItem->parent();
        }

        QString pathToProject = parentItem->data(0, Qt::UserRole).toString();

        removeItem(parentItem);

        provide(pathToProject);

        return;

    } else if (QFileInfo(path + "/" + name).exists()) {
        QMessageBox::critical(this, "Error", "Folder exists in: " + QFileInfo(path + "/" + name).absolutePath());

        return;
    }
    if (!QDir().mkpath(path + "/" + name)) {
        qDebug() << "failed creating folder for projects and files";

        return;
    }

   provide(path + "/" + name, item);

}
void SideBar::addNewFile(QTreeWidgetItem* item, DialogGetPathToFile::SelectExtensions selectedExtensions) {

    QString path = DialogGetPathToFile::getPathToFile(item, this, selectedExtensions);

    if (path.isEmpty()) {

        return;
    }

    if (item != nullptr && !contains(path) && QFileInfo(path).exists() &&
            QMessageBox::question(this, "File exists and it's not part of project",
                                  "Do you wish update your project ?",
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {

        QTreeWidgetItem* parentItem = item;

        while (parentItem->parent()) {

            parentItem = parentItem->parent();
        }

        QString pathToProject = parentItem->data(0, Qt::UserRole).toString();

        removeItem(parentItem);

        provide(pathToProject);

        return;

    } else if (QFileInfo(path).exists()) {
        QMessageBox::critical(this, "Error", "File exists in: " + QFileInfo(path).absolutePath());

        return;
    }

    if (!QFile(path).open(QIODevice::WriteOnly)) {

        qDebug() << "failed creating file :" << path;

        return;
    }

    provide(path, item);
}


void SideBar::renameItem(QTreeWidgetItem* item) {

    QString path = item->data(0, Qt::UserRole).toString();

    if (!QFileInfo(item->data(0, Qt::UserRole).toString()).exists()) {
        QMessageBox::critical(this, "Item not exist!", item->data(0, Qt::UserRole).toString());

        return;
    }

    bool ok;

    QString name = QInputDialog::getText(this, "Rename", "New Name:", QLineEdit::Normal, "", &ok);


    if (!ok || !isValidName(name)) {

        return;
    }


    QString newName;

    if (QFileInfo(item->data(0, Qt::UserRole).toString()).isDir()) {
        newName = QFileInfo(path).absoluteDir().absolutePath() + "/" + name;

    } else {
        newName = QFileInfo(path).absoluteDir().absolutePath() + "/" + name + "." + QFileInfo(path).suffix();
    }

    if (name.isEmpty()) {
        QMessageBox::critical(this, "Error!", "Item name cannot be empty");

        return;
    }

    if (QFileInfo(newName).exists()) {
        QMessageBox::critical(this, "Error!", "this item: " + QFileInfo(newName).baseName() +
                              " already exists in: " + QFileInfo(path).absoluteDir().absolutePath());
        return;
    }

    QStringList renameOldFileList;
    QStringList renameNewFileList;

    QList<QTreeWidgetItem*> list;

    getInsertedItems(renameOldFileList, item);

    if (!QFile::rename(path, newName)) {
        qDebug() << "failed changing name";

        return;
    }

    if (QFileInfo(newName).isDir()) {

        for (int i = 0; item->childCount() > i; i++) {
            list.append(item->child(i));
        }

        item->setText(0, QFileInfo(newName).baseName());

        renameInSidebar(list, item->data(0, Qt::UserRole).toString(), newName);

        item->setData(0, Qt::UserRole, newName);

        getInsertedItems(renameNewFileList, item);

    } else {
        renameOldFileList.append(item->data(0, Qt::UserRole).toString());
        renameNewFileList.append(newName);

        item->setText(0, QFileInfo(newName).baseName() + "." + QFileInfo(path).suffix());
        item->setData(0, Qt::UserRole, newName);
    }


    qDebug() << "renameOldFileList: " << renameOldFileList << "\n";

    qDebug() << "renameNewFileList: " << renameNewFileList << "\n";

    emit renameItemSignal(renameOldFileList, renameNewFileList);
}
void SideBar::deleteItem(QTreeWidgetItem* item) {

    QString path = item->data(0, Qt::UserRole).toString();

    if (QMessageBox::question(this, "Are you sure?", QFileInfo(path).baseName() + " will be deleted.",
                     QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {

        return;
    }

    removeItem(item, false);

    if (!QFileInfo(path).exists()) {

        return;
    }

    if (QFileInfo(path).isDir()) {

        QDir dir(path);

        if (!dir.removeRecursively()) {
            qDebug() << "failed deleting folder by path: " << path;
        }
    }
    else {

        if (!QFile(path).remove()) {
            qDebug() << "failed deleting file by path: " << path;
        }
    }

}
void SideBar::removeItem(QTreeWidgetItem* item, bool isSaveFile) {

    QStringList removeFileList;

    if (!QFileInfo(item->data(0, Qt::UserRole).toString()).exists()) {
        QMessageBox::critical(this, "Item not exist!", item->data(0, Qt::UserRole).toString());

        isSaveFile = false;
    }

    if (item == selectedProject) {
        selectedProject = nullptr;
    }

    QList<QTreeWidgetItem*> list = item->takeChildren();

    if (!list.isEmpty()) {

        removeFromSidebar(list, removeFileList);
    }
    else {
        removeFileList.append(item->data(0, Qt::UserRole).toString());
    }

    delete item;

    emit removeItemSignal(removeFileList, isSaveFile);
}

void SideBar::provide(const QString& path, QTreeWidgetItem* item) {

    //check is valid exten and existing at sideBar
    checkItemAndFillLists(path);
    
    if (!invalidFileExtensionList.isEmpty() ||
        !duplicateList.isEmpty()) {

        DialogUnacceptable* msgBox = new DialogUnacceptable(invalidFileExtensionList.join("\n") + duplicateList.join("\n"), this);

        msgBox->exec();

        delete msgBox;

        invalidFileExtensionList.clear();
        duplicateList.clear();

        return;
    }

    addToSideBar(path, item);

    emit switchToWorkSectionSignal();
}
bool SideBar::contains(const QString& path) {
    QTreeWidgetItemIterator it(this);

    while (*it)
    {
        QTreeWidgetItem* currentItem = *it;
        if (currentItem->data(0, Qt::UserRole) == path)
        {
            return true;
        }
        ++it;
    }
    return false;
}
bool SideBar::isValidName(const QString& name) {

    if (name.contains('\\') || name.contains('/') || name.contains(':') || name.contains('*') || name.contains('?') ||
        name.contains('<') || name.contains('>') || name.contains('|') || name.contains('"')) {
        QMessageBox::critical(this, "Warning", "name cannot have \\, /, :, *, ?, <, >, |, \"");

        return false;
    }

    return true;
}

void SideBar::addToSideBar(const QString& path, QTreeWidgetItem* rootItem) {

    QTreeWidgetItem* item = new QTreeWidgetItem;

    item->setText(0, QFileInfo(path).fileName());
    item->setData(0, Qt::UserRole, path);

    if (QFileInfo(path).isDir()) {

        if (rootItem == nullptr) {
            rootItem = item;

            addTopLevelItem(rootItem);
            createCMakeLists(rootItem);
        }
        else {
            rootItem->addChild(item);

            rootItem = item;
        }

        rootItem->setIcon(0, QIcon(":/sources/img/icons/folder.svg"));

        QStringList fileOrDirPath = QDir(path).entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::NoSort);

        for (QString fileOrDir : fileOrDirPath) {

            addToSideBar(path + "/" + fileOrDir, rootItem);
        }
    }
    else {
        if (rootItem != nullptr) {
            rootItem->addChild(item);
        } else {
            addTopLevelItem(item);
        }

        item->setIcon(0, QIcon(":/sources/img/icons/file.svg"));
    }
}

void SideBar::getInsertedItems(QStringList& list, QTreeWidgetItem* item) {


    for (int i = 0; item->childCount() > i; i++)  {

        list.append(item->child(i)->data(0, Qt::UserRole).toString());

        if (QFileInfo(item->child(i)->data(0, Qt::UserRole).toString()).isDir()) {

            getInsertedItems(list, item->child(i));
        }
    }
}

QTreeWidgetItem* SideBar::getItemByPath(const QString& path) {

    QTreeWidgetItemIterator it(this);

    while (*it)
    {
        QTreeWidgetItem* currentItem = *it;
        if (currentItem->data(0, Qt::UserRole) == path)
        {
            return currentItem;
        }
        ++it;
    }
    return nullptr;
}
void SideBar::removeFromSidebar(QList<QTreeWidgetItem*>& list, QStringList& removeFileList) {

    for (QTreeWidgetItem* i : list) {

        QList<QTreeWidgetItem*> listChild = i->takeChildren();

        if (!listChild.isEmpty()) {
            removeFromSidebar(listChild, removeFileList);
        }
        else {
            removeFileList.append(i->data(0, Qt::UserRole).toString());
        }
        delete i;
    }
}

void SideBar::renameInSidebar(QList<QTreeWidgetItem*>& list, const QString& oldPath, const QString& newPath) {

    for (QTreeWidgetItem* i : list) {

        QList<QTreeWidgetItem*> listChild;

        for (int j = 0; i->childCount() > j; j++) {
            listChild.append(i->child(j));
        }

        i->setData(0, Qt::UserRole, newPath + i->data(0, Qt::UserRole).toString().
                   remove(0, oldPath.length()));


        qDebug() << i->data(0, Qt::UserRole).toString();



        if (QFileInfo(i->data(0, Qt::UserRole).toString()).isDir()) {

            renameInSidebar(listChild, oldPath, newPath);
        }
    }
}

void SideBar::checkItemAndFillLists(const QString& path) {

    if (QFileInfo(path).isDir()) {

        if (contains(path)) {
            duplicateList.append("Dir already in list: " + path);
        }

        QStringList fileOrDirPath = QDir(path).entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::NoSort);

        for (QString fileOrDir : fileOrDirPath) {

            checkItemAndFillLists(path + "/" + fileOrDir);
        }
    }
    else {

        if (contains(path)) {
            duplicateList.append("File already in list: " + path);
        }

        for (const QString& allowedExten : allowedExtension) {

            if (QFileInfo(path).suffix()
                == allowedExten) {

                return;
            }
        }

        invalidFileExtensionList.append("File has unsupported extension : " + path + "\n");
    }
}


void SideBar::dragEnterEvent(QDragEnterEvent* e) {

    if (!e->mimeData()->hasUrls()) {
        return;
    }

    for (const QUrl& url : e->mimeData()->urls()) {

        if (!(QFileInfo(url.toLocalFile()).isDir()) &&
            !(QFileInfo(url.toLocalFile()).suffix() == "txt")) {

            return;
        }
    }

    setStyleSheet("SideBar {border: 3px solid #FBB03B;}");

    e->acceptProposedAction();

    QWidget::dragEnterEvent(e);
}

void SideBar::dragLeaveEvent(QDragLeaveEvent* e) {

    setStyleSheet("SideBar {border: none;}");

    QWidget::dragLeaveEvent(e);
}

void SideBar::dragMoveEvent(QDragMoveEvent* e) {
    e->acceptProposedAction();

    QWidget::dragMoveEvent(e);
}

void SideBar::dropEvent(QDropEvent* e) {

    setStyleSheet("StartSection {border: none;}");

    for (const QUrl& url : e->mimeData()->urls()) {

        provide(url.toLocalFile());
    }

    QWidget::dropEvent(e);
}


void SideBar::createCMakeLists(QTreeWidgetItem* item) {

    QString pathToRootFolder = item->data(0, Qt::UserRole).toString();

    QFile CMakeLists(pathToRootFolder + "/CMakeLists.txt");

    if (!CMakeLists.exists()) {

        if (!CMakeLists.open(QIODevice::WriteOnly)) {
            qDebug() << "failed creating CMakeLists.txt";

            return;
        }

        QProcess cmakeProcess;

        cmakeProcess.start("cmake", QStringList("--version"));

        cmakeProcess.waitForFinished();

        QString output = cmakeProcess.readAll();

        QString CMakeVersion = "VERSION " + output.left(output.indexOf("\n")).remove("cmake version ");

        CMakeLists.write(QString("cmake_minimum_required(" + CMakeVersion + ")\n"
                                 "project(name)\n"
                                 "#main is exe for running\n"
                                 "add_executable(main main.cpp)").toUtf8());

        CMakeLists.close();
    }
}
