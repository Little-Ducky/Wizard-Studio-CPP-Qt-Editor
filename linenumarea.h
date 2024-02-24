#ifndef LINENUMAREA_H
#define LINENUMAREA_H

#include <QWidget>
#include <QPainter>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>
#include <QVBoxLayout>

#include "codeeditor.h"

namespace Ui {
class LineNumArea;
}

class LineNumArea : public QWidget
{
    Q_OBJECT

public:
    explicit LineNumArea(QString path, QWidget *parent = nullptr);
    ~LineNumArea();

private:
    QVBoxLayout* layout;

    CodeEditor* codeEditor;

private:
    void paintEvent(QPaintEvent*) override;

private slots:
    void updateLineNumArea();

public:
    QString getText() const;
    void setText(const QString&);

    void saveFile(const QString&);

    void clearCodeEditor();

    void setRenamedFilePath(const QString&);

    QString filePath;
};

#endif // LINENUMAREA_H
