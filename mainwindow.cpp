#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "edituserinfo.h"
#include "conndb.h"
#include <QDialog>
#include <QtDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->twSide->expandAll();
    ui->swFunc->setCurrentIndex(0);
    setWindowTitle("课堂系统");
    connect(ui->twSide, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(switchPage(QTreeWidgetItem *)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


//根据被点击的菜单项的不同，显示不同的功能界面
void MainWindow::switchPage(QTreeWidgetItem *twi)
{
    QString clicked = twi->text(0);
    if (clicked == "用户信息") {
        ui->swFunc->setCurrentIndex(0);
    } else if (clicked == "创建课程") {
        ui->swFunc->setCurrentIndex(1);
    } else if (clicked == "课程情况") {
        ui->swFunc->setCurrentIndex(2);
    }
}

void MainWindow::getUsID(int usID) { //获得登录用户id
    userID = usID; //为调用displayUsInfo()做准备
    displayUsInfo();    //根据登录用户的ID显示用户信息
}

void MainWindow::displayUsInfo(){
    QSqlQuery query(ConnDB::db);
    QString sql = QString("select us_name, us_phone, us_email, mac, us_rtime from user_info where us_id = %1").arg(userID);
    query.exec(sql);
    if (!query.first())
    {
         QMessageBox::warning(this, tr("Warning"), tr("Database query error."), QMessageBox::Yes);
    }
    else {
        ui->labUserFlag->setText(query.value(0).toString());
        ui->labNameData->setText(query.value(0).toString());
        ui->labPhoneData->setText(query.value(1).toString());
        ui->labEmailData->setText(query.value(2).toString());
        ui->labMacData->setText(query.value(3).toString());
        ui->labRTimeData->setText(query.value(4).toString());
    }
}

void MainWindow::on_btnEditData_clicked()
{
    EditUserInfo *eui = new EditUserInfo(this);
    eui->setUsID(userID);   //将当前登录用户的ID传给该窗口
    eui->exec();
    displayUsInfo();    //重新显示用户信息
}
