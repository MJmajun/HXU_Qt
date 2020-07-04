#ifndef REGISTERDLG_H
#define REGISTERDLG_H

#include <QDialog>

namespace Ui {
class RegisterDlg;
}

class RegisterDlg : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDlg(int rType, QWidget *parent = nullptr);
    ~RegisterDlg();

private slots:
    void on_confirmBtn_clicked();
    void on_closeBtn_clicked();

private:
    Ui::RegisterDlg *ui;
    int regiType;   //注册类型，如果是普通用户注册，值设为1，商家用户注册，值设为2
};

#endif // REGISTERDLG_H
