#ifndef CLEARDIALOG_H
#define CLEARDIALOG_H

#include <QDialog>
#include <QTime>

namespace Ui {
class ClearDialog;
}

class ClearDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClearDialog(QTime startTime, QWidget *parent = nullptr);
    ~ClearDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ClearDialog *ui;
};

#endif // CLEARDIALOG_H
