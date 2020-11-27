#include "commandimpl.h"

#include <QDebug>

CommandImpl::CommandImpl()
{
    this->setParent(QCoreApplication::instance());
}

void CommandImpl::print(QString string)
{
    qDebug() << "TEST : " << string;
}
