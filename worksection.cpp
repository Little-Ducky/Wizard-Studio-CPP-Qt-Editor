#include "worksection.h"
#include "ui_worksection.h"

WorkSection::WorkSection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkSection)
{
    setAttribute(Qt::WA_StyledBackground);

    ui->setupUi(this);
    ui->tabWidget->setAttribute(Qt::WA_StyledBackground);
    ui->tabWidget->setDocumentMode(true);


    connect(ui->tabWidget, &QTabWidget::currentChanged, [=](int index){
        Q_UNUSED(index);

        emit changedTabSignal();
    });
}

WorkSection::~WorkSection()
{
    delete ui;
}


void WorkSection::addToTabWidget(const QString& path) {

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "openning file " + path + "has failed!");

        return;
    }

    LineNumArea* lineNumArea = new LineNumArea(path);

    pathToFileMap.insert(path, lineNumArea);

    ui->tabWidget->addTab(lineNumArea, QFileInfo(path).fileName());

    lineNumArea->setText(file.readAll());

    file.close();

    QPushButton* btnClose = new QPushButton;

    ui->tabWidget->tabBar()->setTabButton(ui->tabWidget->count() - 1, QTabBar::RightSide, btnClose);

    connect(btnClose, &QPushButton::clicked, [=](){
        lineNumArea->saveFile(pathToFileMap.key(lineNumArea));

        ui->tabWidget->removeTab(ui->tabWidget->indexOf(lineNumArea));
        pathToFileMap.remove(pathToFileMap.key(lineNumArea));
    });

}

void WorkSection::removeTabs(const QStringList& removeFileList, bool isSaveFile) {

    for (QString removeFile : removeFileList) {

        if (ui->tabWidget->indexOf(pathToFileMap.value(removeFile)) != -1) {

            if (isSaveFile) {
                static_cast<LineNumArea*>(pathToFileMap.value(removeFile))->saveFile(removeFile);
            }

            ui->tabWidget->removeTab(ui->tabWidget->indexOf(pathToFileMap.value(removeFile)));

            pathToFileMap.remove(removeFile);
        }
    }
}

void WorkSection::renameTabs(const QStringList& renameOldList,
                             const QStringList& renameNewList) {

    for (int i = 0; renameOldList.length() > i; i++) {

        if (ui->tabWidget->indexOf(pathToFileMap.value(renameOldList.at(i))) != -1) {

            ui->tabWidget->setTabText(ui->tabWidget->indexOf(pathToFileMap.value(renameOldList.at(i))),
                                      QFileInfo(renameNewList.at(i)).fileName());

            pathToFileMap.insert(renameNewList.at(i), pathToFileMap.value(renameOldList.at(i)));

            pathToFileMap.remove(renameOldList.at(i));

            static_cast<LineNumArea*>(pathToFileMap.value(renameNewList.at(i)))->setRenamedFilePath(renameNewList.at(i));
        }
    }
}

void WorkSection::setCurrentWidget(QWidget* widget) {
    ui->tabWidget->setCurrentWidget(widget);
}


QWidget* WorkSection::getValue(const QString& key) const {
    return pathToFileMap.value(key);
}

QString WorkSection::getKey(QWidget* value) const {
    return pathToFileMap.key(value);
}

QStringList WorkSection::getKeys() const {
    return pathToFileMap.keys();
}


QString WorkSection::getCurrentPathToFile() const {
    return pathToFileMap.key(ui->tabWidget->currentWidget());
}

QString WorkSection::getCurrentTextInTextEdit() const {

    LineNumArea* lineNumArea = static_cast<LineNumArea*>(ui->tabWidget->currentWidget());

    return lineNumArea->getText();
}

void WorkSection::saveAllOpenedFilesByProject(QTreeWidgetItem* root) {

    QTreeWidgetItemIterator it(root);

    QString path;

    while (*it)
    {
        path = (*it)->data(0, Qt::UserRole).toString();

        if (!QFileInfo(path).isDir() &&
                getValue(path) != nullptr) {

            QFile file(path);

            if (!file.open(QIODevice::WriteOnly)) {

                return;
            }

            file.write(static_cast<LineNumArea*>(getValue(path))->getText().toUtf8());

            file.close();

        }
        ++it;
    }

}
