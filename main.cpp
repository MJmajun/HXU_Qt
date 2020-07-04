#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "logindlg.h"
#include "mainwindow.h"
#include "conndb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!ConnDB::initDB()) {
        return 0;
    }  //连接数据库失败时，直接退出程序

    LogInDlg logInDlg;
    logInDlg.show();

    return a.exec();
}
