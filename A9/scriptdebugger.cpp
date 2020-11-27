 #include "scriptdebugger.h"
 #include "scriptbreakpointinfo.h"

 #include <QtScript/QScriptEngine>
 #include <QtScript/QScriptEngineAgent>
 #include <QtScript/QScriptContextInfo>
 #include <QtScript/QScriptValueIterator>
 #include <QtCore/QTextStream>
 #include <QtCore/QStack>
 #include <QDebug>
 #include <QApplication>
 #include <QThread>

 class ScriptDebuggerPrivate : public QScriptEngineAgent
 {
     Q_DECLARE_PUBLIC(ScriptDebugger)

 public:
     enum Mode {
         Run,
         StepInto
     };

     ScriptDebuggerPrivate(QScriptEngine *engine);
     ~ScriptDebuggerPrivate();

     // QScriptEngineAgent interface
     void scriptLoad(qint64 id, const QString &program,
                     const QString &fileName, int lineNumber);
     void scriptUnload(qint64 id);

     void positionChange(qint64 scriptId,
                         int lineNumber, int columnNumber);

     void functionEntry(qint64 scriptId);
     void functionExit(qint64 scriptId,
                       const QScriptValue &returnValue);

     void exceptionThrow(qint64 scriptId,
                         const QScriptValue &exception, bool hasHandler);

     void setMode(Mode mode);
     Mode mode() const;

     int listLineNumber() const;
     void setListLineNumber(int lineNumber);

     Mode m_mode;

     ScriptDebugger *q_ptr;

     bool interrupted = false;
     bool moveNext = false;
 };

 ScriptDebuggerPrivate::ScriptDebuggerPrivate(QScriptEngine *engine)
     : QScriptEngineAgent(engine), m_mode(Run)
 {
 }

 ScriptDebuggerPrivate::~ScriptDebuggerPrivate()
 {
 }

 void ScriptDebuggerPrivate::setMode(Mode mode)
 {
     m_mode = mode;
 }

 ScriptDebuggerPrivate::Mode ScriptDebuggerPrivate::mode() const
 {
     return m_mode;
 }

 void ScriptDebugger::moveNext()
 {
     d_ptr->moveNext = true;
 }

 void ScriptDebugger::interrupt()
 {
     d_ptr->interrupted = true;
 }

 bool ScriptDebugger::isIntrrupted()
 {
     return d_ptr->interrupted;
 }

 void ScriptDebuggerPrivate::scriptLoad(qint64 id, const QString &program,
                                        const QString &/*fileName*/, int /*lineNumber*/)
 {
     if(!this->interrupted)
        qDebug() << "[ScriptDebugger] [ScriptLoad] ID : " << id << "/ Program : " << program;
 }

 void ScriptDebuggerPrivate::scriptUnload(qint64 id)
 {
     if(!this->interrupted)
        qDebug() << "[ScriptDebugger] [ScriptUnload] ID : " << id;
 }

 void ScriptDebuggerPrivate::functionEntry(qint64 /*scriptId*/)
 {
     if(!this->interrupted)
        qDebug() << "[ScriptDebugger] [Function Entry]";
 }

 void ScriptDebuggerPrivate::functionExit(qint64 /*scriptId*/,
                                          const QScriptValue &returnValue)
 {
     if(!this->interrupted)
        qDebug() << "[ScriptDebugger] [Function Exit] retureValue : " << returnValue.toString();
 }

 void ScriptDebuggerPrivate::positionChange(qint64 scriptId,
                                            int lineNumber, int columnNumber)
 {
     if(!this->interrupted)
        qDebug() << "[ScriptDebugger] [Position Change] ScriptId" << scriptId << " / Linenumber : " << lineNumber << " / Columnnumber : " << columnNumber;

     bool enterInteractiveMode = false;


     //TODO - Breakpoint
     //if (m_bpManager->hasBreakpoints()) {t
     //
     //}

     switch (mode()) {
     case Run:
         break;

     case StepInto:
         enterInteractiveMode = true;
         break;
     }

     if (enterInteractiveMode) { 

         while(!moveNext && !interrupted)
            //THIS IS IMPORTANT! WAIT FOR UI THREAD!
            QApplication::processEvents();

         moveNext = false;
     }

     this->q_ptr->emit signalLineChange(lineNumber);
 }

 void ScriptDebuggerPrivate::exceptionThrow(qint64 scriptId,
                                            const QScriptValue &exception,
                                            bool hasHandler)
 {
     if(!this->interrupted)
        qDebug() << "[ScriptDebugger] [Exception Throw] ScriptId" << scriptId << " / exception : " << exception.toString() << " / hasHandler : " << hasHandler;
 }

 ScriptDebugger::ScriptDebugger(QScriptEngine *engine)
     : d_ptr(new ScriptDebuggerPrivate(engine))
 {
     d_ptr->q_ptr = this;
     engine->setAgent(d_ptr);
 }

 ScriptDebugger::~ScriptDebugger()
 {
     delete d_ptr;
     d_ptr = nullptr;
 }

 void ScriptDebugger::breakAtNextStatement()
 {
     Q_D(ScriptDebugger);
     d->setMode(ScriptDebuggerPrivate::StepInto);
 }

 void ScriptDebugger::setBreakpoint(int lineNumber)
 {
     Q_D(ScriptDebugger);
     //TODO
 }
