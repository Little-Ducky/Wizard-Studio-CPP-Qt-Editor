#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit{parent},
    syntaxisLight(document())
{
    document()->setDocumentMargin(0);

    setAttribute(Qt::WA_StyledBackground);

    setLineWrapMode(LineWrapMode::NoWrap);

    viewport()->setObjectName("viewport");
    viewport()->setAttribute(Qt::WA_StyleSheet);
}

void CodeEditor::paintEvent(QPaintEvent *e) {

    int countBlocks = document()->lastBlock().blockNumber() + 1;

    // need for calculating left margin
    if (countBlocks >= marginByCount) {
        while (countBlocks >= marginByCount * 10) {
            marginByCount *= 10;
            marginMultiplier++;
        }
    }
    else {
        while (countBlocks < marginByCount) {
            marginByCount /= 10;
            marginMultiplier--;
        }
    }

   setViewportMargins(marginMultiplier * QFontMetrics(font()).boundingRect('A').width() + 20, 0, 0, 0);

   QPlainTextEdit::paintEvent(e);
}

void CodeEditor::keyPressEvent(QKeyEvent* e) {

    if (e->key() == Qt::Key_S && (e->modifiers() & Qt::ControlModifier)){

        emit saveFileSignal();

        return;
    }

    if (e->key() == Qt::Key_Return &&
            (e->modifiers() & Qt::ShiftModifier)) {
        QTextCursor cursor = textCursor();
        cursor.insertText("\n");

        verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    }
    else {
        QPlainTextEdit::keyPressEvent(e);
    }
}

CodeEditor::SyntaxisLight::SyntaxisLight(QTextDocument* parent) :
    QSyntaxHighlighter(parent) {

    keywordFormat.setForeground(QColor("#a2d7de"));
    commentFormat.setFontWeight(QFont::Bold);
    commentFormat.setForeground(Qt::gray);

    keywords << "if" << "else" << "while" << "do" << "for" << "class" << "struct" << "namespace"
             << "const" << "int" << "bool" << "char" << "float" << "double" << "signed" << "endif"
             << "unsigned" << "auto" << "return" << "void" << "include" << "define"
             << "ifdef" << "ifndef" << "elif";


    keywordExpression.setPattern("\\b(" + keywords.join("|") + ")\\b");
    commentExpressionSingle.setPattern("//[^\n]*");
    startExpression.setPattern("/\\*");
    endExpression.setPattern("\\*/");

}

void CodeEditor::SyntaxisLight::highlightBlock(const QString& text) {



    QRegularExpressionMatchIterator matchIteratorKeywordExpression = keywordExpression.globalMatch(text);

    // keywords lighting
    while (matchIteratorKeywordExpression.hasNext()) {
        QRegularExpressionMatch match = matchIteratorKeywordExpression.next();
        setFormat(match.capturedStart(), match.capturedLength(), keywordFormat);
    }

    // single comment
    QRegularExpressionMatchIterator matchIteratorCommentSingle = commentExpressionSingle.globalMatch(text);

    while (matchIteratorCommentSingle.hasNext()) {
        QRegularExpressionMatch match = matchIteratorCommentSingle.next();
        setFormat(match.capturedStart(), match.capturedLength(), commentFormat);
    }

    // multiline comment
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1) {
        startIndex = text.indexOf(startExpression);
    }

    while (startIndex >= 0) {

        QRegularExpressionMatch endMatch;
        int endIndex = text.indexOf(endExpression, startIndex, &endMatch);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + endMatch.capturedLength();
        }
        setFormat(startIndex, commentLength, commentFormat);
        startIndex = text.indexOf(startExpression,
                                  startIndex + commentLength);
    }
}
