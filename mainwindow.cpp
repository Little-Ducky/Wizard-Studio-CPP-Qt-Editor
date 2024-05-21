#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cmakeRunConfigProcess = new QProcess;
    cmakeRunBuildProcess = new QProcess;

    #ifdef Q_OS_WIN
        cmakeRunConfigProcess->setProgram("cmd");
        cmakeRunBuildProcess->setProgram("cmd");
    #elif defined(Q_OS_MAC)
        cmakeRunConfigProcess->setProgram("open");
        cmakeRunBuildProcess->setProgram("open");
    #elif defined(Q_OS_LINUX)
        cmakeRunConfigProcess->setProgram("xdg-open");
        cmakeRunBuildProcess->setProgram("xdg-open");
    #else
        qWarning("IDE is not supported on this platform.");
    #endif

    //setup needed widgets
    statusBarText = new QPushButton;

    // setting of program
    dialogProgramSetting = new DialogSettingProgram(this);

    setupStyle(dialogProgramSetting->getTheme());

    ui->statusBar->addWidget(statusBarText);
    statusBarText->setCursor(QCursor(Qt::PointingHandCursor));
    statusBarText->setHidden(true);
    ui->output->hide();

    /* program's logic */

    //connects menu of file
    connect(ui->actionOpenProject, &QAction::triggered, [=] {
        QString path = QFileDialog::getExistingDirectory(this, "Choose directory:", "", QFileDialog::ShowDirsOnly);

        if (path.isEmpty()) {
            return;
        }

        ui->sideBar->provide(path);
    });
    connect(ui->actionOpenFile, &QAction::triggered, [=] {
        QString path = QFileDialog::getOpenFileName(this, "Choose file:", "", "Text files (*.txt)");

        if (path.isEmpty()) {
            return;
        }

        ui->sideBar->provide(path);
    });
    connect(ui->actionSaveFile, &QAction::triggered, [=] {

        QFile file(ui->workSection->getCurrentPathToFile());

        if (file.fileName().isEmpty()) {

            QMessageBox::warning(this, "Warning!", "no opened file for saving");

            return;
        }

        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QMessageBox::critical(this, "Error", "openning file " + file.fileName() + "has failed!");
            return;
        }

        file.write(ui->workSection->getCurrentTextInTextEdit().toUtf8());
        file.close();
    });

    connect(ui->actionNewFile, &QAction::triggered, [=]() {
        ui->sideBar->addNewFile(nullptr, DialogGetPathToFile::SelectExtensions::Disabled);
    });

    connect(ui->actionNewProject, &QAction::triggered, [=]() {
        ui->sideBar->addNewFolder(nullptr);
    });


    //connects menu of view
    connect(ui->actionOutputConsole, &QAction::triggered, [=] {
        ui->output->setVisible(!ui->output->isVisible());
    });

    connect(ui->actionSetting, &QAction::triggered, [=] {
        dialogProgramSetting->show();
    });


    //connects menu if build
    connect(ui->actionRun, &QAction::triggered, [=]() {

        mustRunExe = true;
        hasError = false;

        cmakeRunConfigProcess->close();
        cmakeRunBuildProcess->close();

        ui->output->clearFieldOutput();

       if (ui->sideBar->getSelectedProject() == nullptr) {
           QMessageBox::warning(this, "Warning!", "no selected item");
           return;
       }

       runCMake();
    });

    connect(ui->actionCompile, &QAction::triggered, [=]() {

        mustRunExe = false;
        hasError = false;

        cmakeRunConfigProcess->close();
        cmakeRunBuildProcess->close();

        ui->output->clearFieldOutput();

        if (ui->sideBar->getSelectedProject() == nullptr) {
            QMessageBox::warning(this, "Warning!", "no selected item");

            return;
        }

        runCMake();
    });

    // project setting
    connect(dialogProgramSetting, &DialogSettingProgram::changeThemeSignal, [=](const QString& theme) {
        setupStyle(theme);
    });
    connect(dialogProgramSetting, &DialogSettingProgram::changeFontSignal, [=](const QString& font) {
        setStyleSheet("QWidget { font-family:" + font + ";}" + styleSheetString);
    });

    
    // getting switching tabs change text of statusBar
    connect(ui->workSection, &WorkSection::changedTabSignal, [=] {

        qDebug() << ui->workSection->getCurrentPathToFile();

        statusBarText->setText(ui->workSection->getCurrentPathToFile().replace("/", " > "));

        if (ui->workSection->getCurrentPathToFile().isEmpty()) {
            statusBarText->hide();
            return;
        }

        statusBarText->show();

        statusBarText->disconnect();

        connect(statusBarText, &QPushButton::clicked, [=]() {

            #ifdef Q_OS_WIN
                QProcess::startDetached("explorer", QStringList() << "/select," << QDir::toNativeSeparators(ui->workSection->getCurrentPathToFile()));
            #elif defined(Q_OS_MAC)
                QProcess::startDetached("open", QStringList() << QDir::toNativeSeparators(ui->workSection->getCurrentPathToFile()));
            #elif defined(Q_OS_LINUX)
                QProcess::startDetached("xdg-open", QStringList() << QDir::toNativeSeparators(ui->workSection->getCurrentPathToFile()));
            #else
                qWarning("Opening file explorer is not supported on this platform.");
            #endif

        });

    });
    connect(ui->sideBar, &SideBar::switchToWorkSectionSignal, [=] {
        ui->stackedWidget->setCurrentIndex(1);
    });
    
    // set as current oppened file in tab widget
    connect(ui->sideBar, &SideBar::itemClickedSignal, [=](QTreeWidgetItem *current, int column) {

        QString path = current->data(0, Qt::UserRole).toString();

        if (QFileInfo(path).exists() &&
            QFileInfo(path).isDir()) {

            return;
        }

        if (!QFileInfo(path).exists()) {
            QMessageBox::critical(this, "Error", "item not exists: " + path);

            return;
        }


        if (ui->workSection->getValue(path) != nullptr) {
            ui->workSection->setCurrentWidget(ui->workSection->getValue(path));

            return;
        }

        ui->workSection->addToTabWidget(path);

        // set as current oppened file in tab widget
        ui->workSection->setCurrentWidget(ui->workSection->getValue(path));

    });
    
    // getting drop (file or folder) it adds to sideBar
    connect(ui->startSection, SIGNAL(provideToSidebarSignal(const QString&)),
            ui->sideBar, SLOT(provide(const QString&)));


    connect(ui->sideBar, &SideBar::removeItemSignal, [=](const QStringList& list, bool isSaveFile) {

        ui->workSection->removeTabs(list, isSaveFile);

        // if sideBar is empty switch to startSection
        if (ui->sideBar->topLevelItemCount() == 0) {
            ui->stackedWidget->setCurrentIndex(0);
        }
    });

    connect(ui->sideBar, &SideBar::renameItemSignal, [=](const QStringList& listOldNames,
                                                         const QStringList& listNewNames) {

        if (listOldNames.contains(ui->workSection->getCurrentPathToFile())) {

            int index = listOldNames.indexOf(ui->workSection->getCurrentPathToFile());

            statusBarText->setText(QString(listNewNames.at(index)).replace("/", " > "));
        }

        ui->workSection->renameTabs(listOldNames, listNewNames);
    });



    /* connects for reading status of building */
    connect(cmakeRunConfigProcess, &QProcess::readyReadStandardOutput, [=]() {

        ui->output->setDataOutput(ui->output->getDataOutput() + cmakeRunConfigProcess->readAllStandardOutput());

        ui->output->setBottomVerticalScrollBar();
    });
    connect(cmakeRunConfigProcess, &QProcess::readyReadStandardError, [=]() {

        QString errorString = cmakeRunConfigProcess->readAllStandardError();

        if (!errorString.isEmpty()) {
            hasError = true;
        }

        ui->output->setDataOutput(ui->output->getDataOutput() + errorString);

        ui->output->setBottomVerticalScrollBar();
    });

    connect(cmakeRunBuildProcess, &QProcess::readyReadStandardOutput, [=]() {

        ui->output->setDataOutput(ui->output->getDataOutput() + cmakeRunBuildProcess->readAllStandardOutput());

        ui->output->setBottomVerticalScrollBar();
    });
    connect(cmakeRunBuildProcess, &QProcess::readyReadStandardError, [=]() {

        QString errorString = cmakeRunBuildProcess->readAllStandardError();

        if (!errorString.isEmpty()) {
            hasError = true;
        }

        ui->output->setDataOutput(ui->output->getDataOutput() + errorString);

        ui->output->setBottomVerticalScrollBar();
    });

    // run building when config is made
    connect(this,  &MainWindow::runCMakeMakeBuildSignal, [=]() {

        QString pathToBuild = QFileInfo(ui->sideBar->getSelectedProject()->data(0, Qt::UserRole).toString()).absolutePath() + "/build " +
                QFileInfo(ui->sideBar->getSelectedProject()->data(0, Qt::UserRole).toString()).baseName();

        QString pathToProject = ui->sideBar->getSelectedProject()->data(0, Qt::UserRole).toString();

        #ifdef Q_OS_WIN
            cmakeRunBuildProcess->setArguments({"/c", "cd", "/d", pathToProject, "&&", "cmake", "--build", pathToBuild, "--config", dialogProgramSetting->getSelectedTypeBuild()});
        #elif defined(Q_OS_MAC)
            cmakeRunBuildProcess->setArguments({"-c", "cd", pathToProject, "&&", "cmake", "--build", pathToBuild, "--config", dialogProgramSetting->getSelectedTypeBuild()});
        #elif defined(Q_OS_LINUX)
            cmakeRunBuildProcess->setArguments({"-c", "cd", pathToProject, "&&", "cmake", "--build", pathToBuild, "--config", dialogProgramSetting->getSelectedTypeBuild()});
        #else
            qWarning("is not supported on this platform.");
        #endif

        cmakeRunBuildProcess->start();
    });

    connect(cmakeRunConfigProcess, &QProcess::finished, [=]() {

        emit runCMakeMakeBuildSignal();
    });
    // run compiled exe
    connect(cmakeRunBuildProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus) {

           if (exitStatus == QProcess::NormalExit && mustRunExe && !hasError) {
                runCompiledExe();
            }
    });

}

MainWindow::~MainWindow()
{
    delete cmakeRunConfigProcess;
    delete cmakeRunBuildProcess;

    delete ui;
}

void MainWindow::setupStyle(const QString& theme) {

    QFile fileStyle(":/sources/style/" + theme + "ThemeStyle.css");

        if (!fileStyle.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, "Error", "openning main file style failed");
        }

    styleSheetString = fileStyle.readAll();

    setStyleSheet("QWidget { font-family:" + dialogProgramSetting->getFont() + ";}" + styleSheetString);

    fileStyle.close();
}

void MainWindow::closeEvent(QCloseEvent* e) {

    ui->workSection->removeTabs(ui->workSection->getKeys());

    e->accept();
}

void MainWindow::runCMake() {

    ui->workSection->saveAllOpenedFilesByProject(ui->sideBar->getSelectedProject());

    ui->output->setVisible(true);

    QString pathToBuild = QFileInfo(ui->sideBar->getSelectedProject()->data(0, Qt::UserRole).toString()).absolutePath() + "/build " +
            QFileInfo(ui->sideBar->getSelectedProject()->data(0, Qt::UserRole).toString()).baseName();

    QString pathToProject = ui->sideBar->getSelectedProject()->data(0, Qt::UserRole).toString();

    QDir dir(pathToBuild);

    if (dir.exists()) {
        if (dir.removeRecursively()) {
            qDebug() << "Folder deleted successfully";
        }
    }

    if (dir.mkpath(pathToBuild)) {
        qDebug() << "Folder created successfully";
    }
    if (dialogProgramSetting->getSelectedCompiler() == "MSVC x64 2022") {


        #ifdef Q_OS_WIN
            cmakeRunConfigProcess->setArguments({"/c", "cd", "/d", pathToBuild, "&&", "cmake", pathToProject});
        #elif defined(Q_OS_MAC)
            arguments = {"-c", "cd", pathToBuild, "&&", "cmake", pathToProject};
        #elif defined(Q_OS_LINUX)
            arguments = {"-c", "cd", pathToBuild, "&&", "cmake", pathToProject};
        #else
            qWarning("is not supported on this platform.");
        #endif

    }
    else {

        #ifdef Q_OS_WIN
            cmakeRunConfigProcess->setArguments({"/c", "cd", "/d", pathToBuild, "&&", "cmake", "-G", "MinGW Makefiles", pathToProject});
        #elif defined(Q_OS_MAC)
            cmakeRunConfigProcess->setArguments({"-c", "cd", pathToBuild, "&&", "cmake", "-G", "Unix Makefiles", pathToProject});
        #elif defined(Q_OS_LINUX)
            cmakeRunConfigProcess->setArguments({"-c", "cd", pathToBuild, "&&", "cmake", "-G", "Unix Makefiles", pathToProject});
        #else
            qWarning("is not supported on this platform.");
        #endif

        QMessageBox::warning(this, "Warning!", "Building with CMake + Mingw must have only latin chars in paths");
    }

    cmakeRunConfigProcess->start();
}
void MainWindow::runCompiledExe() {

    QString pathToBuild = QFileInfo(ui->sideBar->getSelectedProject()->data(0, Qt::UserRole).toString()).absolutePath() + "/build " +
            QFileInfo(ui->sideBar->getSelectedProject()->data(0, Qt::UserRole).toString()).baseName();

    if (dialogProgramSetting->getSelectedCompiler() == "MSVC x64 2022") {

        #ifdef Q_OS_WIN
            QProcess::startDetached("cmd", {"/c", "start", "cmd", "/k", pathToBuild + "/" + dialogProgramSetting->getSelectedTypeBuild() + "/main.exe"});
        #elif defined(Q_OS_MAC)
            QProcess::startDetached("open", {pathToBuild + "/" + dialogProgramSetting->getSelectedTypeBuild() + "/main.exe"});
        #elif defined(Q_OS_LINUX)
            QProcess::startDetached("xdg-open", {pathToBuild + "/" + dialogProgramSetting->getSelectedTypeBuild() + "/main.exe"});
        #else
            qWarning("is not supported on this platform.");
        #endif

    } else {
        #ifdef Q_OS_WIN
            QProcess::startDetached("cmd", {"/c", "start", "cmd", "/k", pathToBuild + "/main.exe"});
        #elif defined(Q_OS_MAC)
            QProcess::startDetached("open", {pathToBuild + "/main.exe"});
        #elif defined(Q_OS_LINUX)
            QProcess::startDetached("xdg-open", {pathToBuild + "/main.exe"});
        #else
            qWarning("is not supported on this platform.");
        #endif
    }

}
