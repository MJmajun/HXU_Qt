#include "registerdlg.h"
#include "ui_registerdlg.h"
#include "conndb.h"
//#include <Qt>
#include <QString>
#include <QFont>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

RegisterDlg::RegisterDlg(int rType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDlg)
{
    regiType = rType;
    ui->setupUi(this);
    setWindowTitle("注册");
    ui->labReg->setAlignment(Qt::AlignHCenter);
    QFont font("Microsoft YaHei", 20, 75);
    ui->labReg->setFont(font);
    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    ui->checPwdLineEdit->setEchoMode(QLineEdit::Password);
}

RegisterDlg::~RegisterDlg()
{
    delete ui;
}

void RegisterDlg::on_closeBtn_clicked()
{
    close();
}

void RegisterDlg::on_confirmBtn_clicked()
{
    //从注册界面获取用户填写的各项注册信息
    QString userName = ui->userNameLineEdit->text().trimmed();
    QString userPhone = ui->userPhoneLineEdit->text().trimmed();
    QString pwd = ui->pwdLineEdit->text().trimmed();
    QString checPwd = ui->checPwdLineEdit->text().trimmed();

    //判断是否有用户没有填写的信息项
    if (userName.isEmpty() || userPhone.isEmpty()
            || pwd.isEmpty() || checPwd.isEmpty()) {
        QMessageBox::warning(this, tr("Waring"), tr("Please fill in all the information!"), QMessageBox::Yes);
        return;
    }

    //判断两次输入的密码是否一致
    if (pwd != checPwd) {
        QMessageBox::warning(this, tr("Waring"), tr("The passwords you typed don't match."), QMessageBox::Yes);
        return;
    }

    QSqlQuery query(ConnDB::db);
    QString sql;

    if (regiType == 1) {    //根据注册类型，需要不同的sql语句
        sql = QString("select us_id from user_info where us_name = '%1'").arg(userName);
    } else if (regiType == 2) {
        sql = QString("select company_id from company_info where company_name = '%1'").arg(userName);
    }
    query.exec(sql);
    if (query.first()) {    //该用户名已被注册
        QMessageBox::warning(this, tr("Warning"), tr("User name existed!"), QMessageBox::Yes);
        return;
    }

    if (regiType == 1) {    //根据注册类型，需要不同的sql语句
        sql = QString("select us_id from user_info where us_phone = '%1'").arg(userPhone);
    } else if (regiType == 2) {
        sql = QString("select company_id from company_info where us_phone = '%1'").arg(userPhone);
    }
    query.exec(sql);
    if (query.first()) {    //电话号码已存在
        QMessageBox::warning(this, tr("Warning"), tr("Phone number existed!"), QMessageBox::Yes);
        return;
    }

    //注册信息都已填写完毕且正确，则将注册信息写入数据库
    QDateTime curDT = QDateTime::currentDateTime();
    if (regiType == 1) {    //根据注册类型，需要不同的sql语句
        sql = QString("insert into user_info(us_name, us_pwd, us_phone, us_rtime) "
                      "value('%1', '%2', '%3', '%4')").arg(userName).arg(pwd).arg(userPhone).arg(curDT.toString("yyyy-MM-dd hh:mm:ss"));
    } else if (regiType == 2) { //一些不重要的字段直接插入空字符串''
        sql = QString("insert into company_info(company_name, company_pwd, company_logo, company_boss, company_addr, company_phone, company_email, company_rtime) "
                      "value('%1', '%2', '', '', '', '%3', '', '%4')").arg(userName).arg(pwd).arg(userPhone).arg(curDT.toString("yyyy-MM-dd hh:mm:ss"));
    }
    if (query.exec(sql)) {
        QMessageBox::information(this, tr("Infomation"), tr("Successful registration."), QMessageBox::Yes);
        close();
    }
}

