#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QPainter>
#include <QTextBlock>
#include <QCompleter>

#include "highlighter.h"
#include "scriptdebugger.h"

/**
 * @brief The CodeEditor class
 */
class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void lineHighlighter(int line);

    void setCompleter(QCompleter *c);
    QCompleter *completer() const;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void insertCompletion(const QString &completion);


private:
    QWidget *lineNumberArea;
    Highlighter *highlighter;
    QString textUnderCursor() const;
    QCompleter *c = nullptr;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
