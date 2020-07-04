#include "chosysdlg.h"
#include "ui_chosysdlg.h"
#include "mainwindow.h"
#include <QMessageBox>

ChoSysDlg::ChoSysDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoSysDlg)
{
    ui->setupUi(this);
    ui->rbtMeetSys->setChecked(true);  //默认选中课堂系统
    setWindowTitle("选择模块");
}

ChoSysDlg::~ChoSysDlg()
{
    delete ui;
}


void ChoSysDlg::on_btnExit_clicked()
{
    close();
}

void ChoSysDlg::on_btnConfirm_clicked()
{
    if (ui->rbtClassSys->isChecked()) { //用户选中课堂系统
        emit postRes(1);    //向登录窗口传送结果，1代表课堂系统
        close();
    }
    else if (ui->rbtMeetSys->isChecked()) { //用户选择会议系统
//        QMessageBox::warning(this, tr("warning"), tr("The conference system has not yet been completed."), QMessageBox::Yes);
        emit postRes(2);    //向登录窗口传送结果，2代表课堂系统
        close();
    }else {
        QMessageBox::warning(this, tr("warning"), tr("You should choose a system to entry."), QMessageBox::Yes);
    }
    return;
}
