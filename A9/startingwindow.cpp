#include "startingwindow.h"
#include "ui_startingwindow.h"

startingWindow::startingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::startingWindow)
{
    ui->setupUi(this);
}

startingWindow::~startingWindow()
{
    delete ui;
}
