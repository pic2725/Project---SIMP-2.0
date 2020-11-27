#include "cleardialog.h"
#include "ui_cleardialog.h"

ClearDialog::ClearDialog(QTime startTime, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClearDialog)
{
    ui->setupUi(this);

    qint64 millisecondsDiff = startTime.msecsTo(QTime::currentTime());

    ui->lbTime->setText(QString::number(millisecondsDiff / 1000) + " seconds");
}

ClearDialog::~ClearDialog()
{
    delete ui;
}

void ClearDialog::on_pushButton_clicked()
{
    QApplication::quit();
}
