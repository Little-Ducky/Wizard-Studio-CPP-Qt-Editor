
#include "linenumarea.h"

LineNumArea::LineNumArea(QString path, QWidget *parent) :
    QWidget(parent),
    filePath(path)
{
    setAttribute(Qt::WA_StyledBackground);

    layout = new QVBoxLayout;

    codeEditor = new CodeEditor;

    layout->addWidget(codeEditor);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);

    connect(codeEditor->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumArea()));
    connect(codeEditor, SIGNAL(textChanged()), this, SLOT(updateLineNumArea()));

    connect(codeEditor, &CodeEditor::saveFileSignal, [=]{
        QFile file(filePath);

        if (file.fileName().isEmpty()) {
            return;
        }

        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QMessageBox::critical(this, "Error", "openning file " + file.fileName() + "has failed!");
            return;
        }

        file.write(codeEditor->toPlainText().toUtf8());
        file.close();
    });
}

void LineNumArea::paintEvent(QPaintEvent* e) {

    QPainter painter(this);

    QFont font(codeEditor->font());

    int heightBlock = codeEditor->cursorRect(codeEditor->textCursor()).height();
    int lastBlockNumber = codeEditor->document()->lastBlock().blockNumber();
    int verScrollValue = codeEditor->verticalScrollBar()->value();

    double ascent = (QFontMetricsF(font).ascent());

    for (int firstVisibleBlock = verScrollValue, i = 0;
         lastBlockNumber >= i + verScrollValue &&
         codeEditor->viewport()->height() > i * heightBlock; i++) {

        painter.drawText(QPointF(5, ascent + i * heightBlock), QString::number(++firstVisibleBlock));
    }

    QWidget::paintEvent(e);
}

void LineNumArea::updateLineNumArea() {
    update();
}

void LineNumArea::setRenamedFilePath(const QString& path) {
    filePath = path;
}

QString LineNumArea::getText() const {
    return codeEditor->toPlainText();
}

void LineNumArea::setText(const QString& text) {
    codeEditor->setPlainText(text);
}

void LineNumArea::saveFile(const QString& path) {

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "openning file " + QFileInfo(path).fileName() + "has failed!");

        return;
    }

    QString text = file.readAll();

    file.close();

    if (codeEditor->toPlainText() != text.replace("\r\n", "\n") &&
           QMessageBox::Yes == QMessageBox::question(this, "Question", "Do you want to save " + QFileInfo(path).fileName() + "?", QMessageBox::Yes | QMessageBox::No)) {

        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, "Error", "openning file " + QFileInfo(path).fileName() + "has failed!");

            return;
        }

        file.write(codeEditor->toPlainText().toUtf8());

        file.close();
    }

}

LineNumArea::~LineNumArea() {}

void LineNumArea::clearCodeEditor() {
    codeEditor->clear();
}
