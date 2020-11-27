#ifndef COMMANDIMPL_H
#define COMMANDIMPL_H

#include <QObject>
#include <QCoreApplication>

class CommandImpl : public QObject
{
    Q_OBJECT

public:
    CommandImpl();

    Q_INVOKABLE void print(QString string);
};

#endif // COMMANDIMPL_H
