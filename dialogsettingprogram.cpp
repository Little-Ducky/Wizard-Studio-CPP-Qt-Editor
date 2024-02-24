#include "dialogsettingprogram.h"
#include "ui_dialogsettingprogram.h"

DialogSettingProgram::DialogSettingProgram( QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettingProgram)
{
    ui->setupUi(this);

    setupJson();

    connect(ui->listWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item){

        if (item->text() == "Interface options") {

            ui->stackedWidget->setCurrentIndex(0);
        } else if (item->text() == "Build tools") {

            ui->stackedWidget->setCurrentIndex(1);
        }

    });

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, [&] {

        Ui::FormOptionBuild* formBuildUI = ui->formOptionBuild->ui;
        Ui::FormOptionView* formViewUI = ui->formOptionView->ui;

         if (formViewUI->comboBoxTheme->currentText() != jsonObject["Theme"].toString()) {
            emit changeThemeSignal(formViewUI->comboBoxTheme->currentText());

            updateJson("Theme", formViewUI->comboBoxTheme->currentText());
         }

         if (formViewUI->comboBoxFont->currentText() != jsonObject["Font"].toString()) {
            emit changeFontSignal(formViewUI->comboBoxFont->currentText());

            updateJson("Font", formViewUI->comboBoxFont->currentText());
         }

         if (formBuildUI->comboBoxCompiler->currentText() != jsonObject["Compiler"].toString()) {

            updateJson("Compiler", formBuildUI->comboBoxCompiler->currentText());
         }

         if (formBuildUI->comboBoxTypeBuild->currentText() != jsonObject["Type of build"].toString()) {

            updateJson("Type of build", formBuildUI->comboBoxTypeBuild->currentText());
         }

    });

    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, [=] {
        cancelChangeOptions();
    });

}

DialogSettingProgram::~DialogSettingProgram()
{
    delete ui;
}

void DialogSettingProgram::closeEvent(QCloseEvent* e) {

    cancelChangeOptions();

    e->accept();
}

void DialogSettingProgram::cancelChangeOptions() {

    Ui::FormOptionBuild* formBuildUI = ui->formOptionBuild->ui;
    Ui::FormOptionView* formViewUI = ui->formOptionView->ui;

    formViewUI->comboBoxTheme->setCurrentText(jsonObject["Theme"].toString());
    formViewUI->comboBoxFont->setCurrentText(jsonObject["Font"].toString());

    formBuildUI->comboBoxCompiler->setCurrentText(jsonObject["Compiler"].toString());
    formBuildUI->comboBoxTypeBuild->setCurrentText(jsonObject["Type of build"].toString());
}

QString DialogSettingProgram::getTheme() const {
    return ui->formOptionView->ui->comboBoxTheme->currentText();
}

QString DialogSettingProgram::getFont() const {
    return ui->formOptionView->ui->comboBoxFont->currentText();
}

void DialogSettingProgram::setupJson() {
    QFile settingJson(QCoreApplication::applicationDirPath() + "/Setting.json");

    //default options
    QString theme = "Dark";
    QString font = "Consolas";
    QString compiler = "MSVC x64 2022";
    QString typeBuild = "Debug";

    //default options
    jsonObject["Theme"] = theme;
    jsonObject["Font"] = font;
    jsonObject["Compiler"] = compiler;
    jsonObject["Type of build"] = typeBuild;

    Ui::FormOptionView* formViewUI = ui->formOptionView->ui;
    Ui::FormOptionBuild* formBuildUI = ui->formOptionBuild->ui;

    formViewUI->comboBoxTheme->setCurrentText(jsonObject["Theme"].toString());
    formViewUI->comboBoxFont->setCurrentText(jsonObject["Font"].toString());

    formBuildUI->comboBoxCompiler->setCurrentText(jsonObject["Compiler"].toString());
    formBuildUI->comboBoxTypeBuild->setCurrentText(jsonObject["Type of build"].toString());

    if (!settingJson.exists()) {

        if (!settingJson.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, "Error", "creating file setting json failed!");
            return;
        }

        settingJson.write(QJsonDocument(jsonObject).toJson());

    } else {
        if (!settingJson.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, "Error", "openning file setting json failed!");
            return;
        }

        QJsonDocument settingJsonDocument = QJsonDocument::fromJson(QByteArray(settingJson.readAll()));

        if (settingJsonDocument.isNull()) {
            QMessageBox::critical(this, "Error", "json document settingJsonDocument is not valid or empty!");
        }

        jsonObject = settingJsonDocument.object();

        formViewUI->comboBoxTheme->setCurrentText(jsonObject["Theme"].toString());
        formViewUI->comboBoxFont->setCurrentText(jsonObject["Font"].toString());

        formBuildUI->comboBoxCompiler->setCurrentText(jsonObject["Compiler"].toString());
        formBuildUI->comboBoxTypeBuild->setCurrentText(jsonObject["Type of build"].toString());
    }

    settingJson.close();
}

void DialogSettingProgram::updateJson(const QString& key, const QString& value) {

    QFile settingJson(QCoreApplication::applicationDirPath() + "/Setting.json");

    if (!settingJson.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "openning file setting json failed!");

        return;
    }

    QJsonDocument settingJsonDocument = QJsonDocument::fromJson(QByteArray(settingJson.readAll()));

    if (settingJsonDocument.isNull()) {
        QMessageBox::critical(this, "Error", "json document settingJsonDocument is not valid or empty!");

        return;
    }

    jsonObject = settingJsonDocument.object();

    jsonObject[key] = value;

    settingJson.close();

    if (!settingJson.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::critical(this, "Error", "openning file setting json failed!");

        return;
    }

    settingJson.write(QJsonDocument(jsonObject).toJson());

    settingJson.close();
}

QString DialogSettingProgram::getSelectedCompiler() {
    return ui->formOptionBuild->ui->comboBoxCompiler->currentText();
}

QString DialogSettingProgram::getSelectedTypeBuild() {
    return ui->formOptionBuild->ui->comboBoxTypeBuild->currentText();
}
