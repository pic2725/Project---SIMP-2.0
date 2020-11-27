#include "mainwindow.h"
#include "window.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GameManager gameEngine;
    window w(&gameEngine);
    w.show();

    return a.exec();
}
