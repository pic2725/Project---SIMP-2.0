#include "codeeditor.h"
#include "codeeditor.h"
#include <QStringList>

#include <QAbstractItemView>
#include <QScrollBar>
#include <QDebug>

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent), c(nullptr)
{
    // Set Editor background and text color
    QPalette p;
    p.setColor(QPalette::Base, Qt::white);
    p.setColor(QPalette::Text, Qt::black);
    this->setPalette(p);

    // Create syntax highlither and linenumber area
    highlighter = new Highlighter(this->document());
    lineNumberArea = new LineNumberArea(this);

    // Set font
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(14);
    this->setFont(font);

    // Set tab size.
    QFontMetrics metrics(font);
    this->setTabStopWidth(4 * metrics.horizontalAdvance(' '));

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::lineHighlighter(int line)
{
    // line start 1
    line--;

    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;

    QStringList lines = this->toPlainText().split("\n");


    int startPos = 0;
    int endPos = 0;


    //get startPos
    for(int i = 0; i < line; i++)
    {
        startPos += lines[i].size();
    }

    //get endPos
    endPos = startPos + lines[line].size();

    qDebug() << "[lineHighlighter] line : " << line << " / startPos : " << startPos << "/ endPos : " << endPos;


    //highlighter setting
    QColor lineColor = QColor(Qt::green).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);

    QTextCursor tempC = textCursor();
    tempC.setPosition(startPos);
    tempC.setPosition(endPos, QTextCursor::MoveAnchor);
    setTextCursor(tempC);

    selection.cursor = tempC;

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);

    selection.cursor.clearSelection();
    extraSelections.append(selection);



    setExtraSelections(extraSelections);
}
void CodeEditor::setCompleter(QCompleter *completer)
{
    if (c)
        QObject::disconnect(c, nullptr, this, nullptr);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, QOverload<const QString &>::of(&QCompleter::activated),
                     this, &CodeEditor::insertCompletion);
}

QCompleter* CodeEditor::completer() const
{
    return c;
}

void CodeEditor::insertCompletion(const QString& completion)
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();

    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);

    if(completion.right(2) == "()")
    {
        tc = textCursor();
        tc.movePosition(QTextCursor::Left);
        setTextCursor(tc);
    }

    qDebug() << "[CodeEditor] [Completion] " << completion;
}

QString CodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void CodeEditor::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
        default:
            break;
        }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);
    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);

    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 2
                        || eow.contains(e->text().right(1)))) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}
