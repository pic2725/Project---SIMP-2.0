#ifndef DEBUGGERTHREAD_H
#define DEBUGGERTHREAD_H

#include <QThread>
#include <QScriptEngine>
#include <QScriptEngineDebugger>

class DebuggerThread : public QThread
{
    Q_OBJECT

public:
    // constructor
    // set name using initializer
    explicit DebuggerThread(QScriptEngine *engine, QString command);

    // overriding the QThread's run() method
    void run() override;
private:
    QScriptEngine *engine;
    QString command;
};

#endif // DEBUGGERTHREAD_H
