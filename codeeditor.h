#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QScrollBar>
#include <QRegularExpression>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);

private:
    void keyPressEvent(QKeyEvent*) override;
    void paintEvent(QPaintEvent*) override;

signals:
    void saveFileSignal();

private:
    int marginByCount = 1;
    int marginMultiplier = 1;

private:

    class SyntaxisLight : public QSyntaxHighlighter {
    public:
        SyntaxisLight(QTextDocument*);

    private:

        void highlightBlock(const QString&) override;
        void applySyntaxHighlighting(const QString &text, int startIndex, int endIndex);
    private:

        QStringList keywords;

        QTextCharFormat keywordFormat;
        QTextCharFormat commentFormat;

        QRegularExpression keywordExpression;
        QRegularExpression commentExpressionSingle;

        QRegularExpression startExpression;
        QRegularExpression endExpression;
    };

    SyntaxisLight syntaxisLight;

};

#endif // CODEEDITOR_H
