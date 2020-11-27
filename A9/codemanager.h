#ifndef CODEMANAGER_H
#define CODEMANAGER_H

#include <QObject>
#include <QAction>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QThread>
#include <QTimer>

#include "commandimpl.h"
#include "scriptdebugger.h"
#include "debuggerthread.h"
#include "gamemanager.h"

class CodeManager : public QObject
{
    Q_OBJECT

public:
    CodeManager(GameManager *gameEngine);
    ~CodeManager();

    void run(QString script, int delay);
    void debug(QString script);
    void moveNextLine();

private:
    GameManager *gameEngine = nullptr;
    QScriptEngine *engine = nullptr;
    ScriptDebugger *debugger = nullptr;
    CommandImpl *commandImpl = nullptr;
    QString script;

    bool isDebug = false;

    void initalizeForDebugging();
    void deinitalizeForDebugging();

    void scriptRun(QString script);

public slots:
    void onDebugProcess();
    void onRunningProcess();
    void onLineNumberChanged(int currentLine);
    void onAnimationFinished();
    void onInterrupted();

signals:
    void signalLineChanged(int currentLine);
    void signalException(const QString errorMessage);
    void signalFinish();
};



#endif // CODEMANAGER_H
