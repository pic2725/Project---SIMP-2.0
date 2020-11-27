#include "debuggerthread.h"

#include <QTextStream>
#include <scriptdebugger.h>

DebuggerThread::DebuggerThread(QScriptEngine *engine, QString command)
{
    this->engine = engine;
    this->command = command;
}

void DebuggerThread::run()
{
    //QTextStream out(stdout, QIODevice::WriteOnly);
    //QTextStream in(stdin);

    ScriptDebugger *testDebugger = new ScriptDebugger(engine);
    //testDebugger->setOutputStream(&out);
    //testDebugger->setInputStream(&in);
    testDebugger->breakAtNextStatement();

    QScriptValue result = engine->evaluate(command);

    if (engine->hasUncaughtException()) {
        //int line = engine->uncaughtExceptionLineNumber();
        //qDebug() << "uncaught exception at line" << line << ":" << result.toString();
    }
}
