#include "startsection.h"
#include "ui_startsection.h"

StartSection::StartSection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartSection)
{
    setAttribute(Qt::WA_StyledBackground);

    ui->setupUi(this);
}

StartSection::~StartSection()
{
    delete ui;
}

void StartSection::dragEnterEvent(QDragEnterEvent* e) {

    if (!e->mimeData()->hasUrls()) {
        return;
    }

    for (const QUrl& url : e->mimeData()->urls()) {

        if (!(QFileInfo(url.toLocalFile()).isDir()) &&
            !(QFileInfo(url.toLocalFile()).suffix() == "txt")) {

            return;
        }
    }

    setStyleSheet("StartSection {border: 3px solid #FBB03B;}");

    e->acceptProposedAction();

    QWidget::dragEnterEvent(e);
}

void StartSection::dragLeaveEvent(QDragLeaveEvent* e) {

    setStyleSheet("StartSection {border: none;}");

    QWidget::dragLeaveEvent(e);
}


void StartSection::dragMoveEvent(QDragMoveEvent* e) {
    e->acceptProposedAction();

    QWidget::dragMoveEvent(e);
}
void StartSection::dropEvent(QDropEvent* e) {

    setStyleSheet("StartSection {border: none;}");

    for (const QUrl& url : e->mimeData()->urls()) {

        emit provideToSidebarSignal(url.toLocalFile());
    }

    QWidget::dropEvent(e);
}
