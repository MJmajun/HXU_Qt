#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include "chosysdlg.h"
#include "mainwindow.h"
#include "meetsys.h"

namespace Ui {
class LogInDlg;
}

class LogInDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LogInDlg(QWidget *parent = nullptr);
    ~LogInDlg();

signals:
    void posUsID(int usID);

private slots:
    void on_logInBtn_clicked();

    void on_exitBtn_clicked();

    void on_registerBtn_clicked();

    void getRes(int res);   //从系统选择界面获得系统选择的结果

private:
    Ui::LogInDlg *ui;
    ChoSysDlg *choSysDlg;
    MainWindow *classSys;
    MeetSys *meetSys;
    int userID; //登录成功的用户的ID
};

#endif // LOGINDLG_H
