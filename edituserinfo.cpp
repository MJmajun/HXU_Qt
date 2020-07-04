#include "edituserinfo.h"
#include "ui_edituserinfo.h"
#include "conndb.h"
#include <QtDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

EditUserInfo::EditUserInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditUserInfo),
    userID(0)
{
    ui->setupUi(this);
}

EditUserInfo::~EditUserInfo()
{
    delete ui;
}

void EditUserInfo::setUsID(int usID)
{
    userID = usID;
}


void EditUserInfo::on_btnClose_clicked()
{
    close();
}

void EditUserInfo::on_btnSubmit_clicked()
{
    //获取用户输入的各项信息
    QString name = ui->leName->text().trimmed();
    QString phone = ui->lePhone->text().trimmed();
    QString email = ui->leEmail->text().trimmed();
    QString mac = ui->leMac->text().trimmed();
    QString pwd = ui->lePwd->text().trimmed();
    QString pwdChe = ui->lePwdChe->text().trimmed();

    //判断
    if (name.isEmpty() && phone.isEmpty() && email.isEmpty()
            && mac.isEmpty() && pwd.isEmpty() && pwdChe.isEmpty()) {    //所有输入项都为空，给出提示
        QMessageBox::information(this, tr("Infomation"), tr("No user information has been changed."));
    } else {    //有输入项不为空， 则分别进行判断是否为空，不为空的进行信息修改操作
        QString sql;
        QSqlQuery query(ConnDB::db);

        //检查输入信息的合法性
        if (!name.isEmpty()) {  //检查用户名是否已存在
            sql = QString("select us_name from user_info where us_name = '%1'").arg(name);
            query.exec(sql);
            if (query.first())  {
                QMessageBox::warning(this, tr("Warning"), tr("User name existed!"), QMessageBox::Yes);
                return;
            }
        }else if (!phone.isEmpty()){    //检查手机号是否已存在
            sql = QString("select us_phone from user_info where us_phone = '%1'").arg(phone);
            query.exec(sql);
            if (query.first())  {
                QMessageBox::warning(this, tr("Warning"), tr("User phone number existed!"), QMessageBox::Yes);
                return;
            }
        }else if (!email.isEmpty()) {   //检查邮箱地址是否已存在
            sql = QString("select us_email from user_info where us_email = '%1'").arg(email);
            query.exec(sql);
            if (query.first())  {
                QMessageBox::warning(this, tr("Warning"), tr("User email existed!"), QMessageBox::Yes);
                return;
            }
        }else if (!pwd.isEmpty()) { //检查两次密码是否都已经输入且完全一致
            if (pwd != pwdChe) {
                QMessageBox::warning(this, tr("Warning"), tr("The passwords you typed don't match."), QMessageBox::Yes);
                return;
            }
        }


        //判断各项输入是否为空，为空则不进行修改，不为空则进行信息的修改
        if (name.isEmpty()) {   //新用户名为空
            ui->leName->clear();
        } else {    //新用户名不为空
            //修改用户名
            sql = QString("update user_info set us_name = '%1' where us_id = %2").arg(name).arg(userID);
            query.exec(sql);
        }
        if (phone.isEmpty()) {   //新手机号为空
            ui->leName->clear();
        } else {    //新手机号不为空
            //修改手机号
            sql = QString("update user_info set us_phone = '%1' where us_id = %2").arg(phone).arg(userID);
            query.exec(sql);
        }
        if (email.isEmpty()) {   //新邮箱地址为空
            ui->leName->clear();
        } else {    //新邮箱地址不为空
            //修改邮箱地址
            sql = QString("update user_info set us_email = '%1' where us_id = %2").arg(email).arg(userID);
            query.exec(sql);
        }
        if (mac.isEmpty()) {   //新Mac地址为空
            ui->leName->clear();
        } else {    //新Mac地址不为空
            //修改Mac地址
            sql = QString("update user_info set mac = '%1' where us_id = %2").arg(mac).arg(userID);
            query.exec(sql);
        }
        if (pwd.isEmpty() && pwdChe.isEmpty()) {  //新密码为空
            ui->lePwd->clear();
            ui->lePwdChe->clear();
        }else if (pwd == pwdChe) {  //两次输入的密码匹配成功，将新密码写入数据库
            sql = QString("update user_info set us_pwd = '%1' where us_id = %2").arg(pwd).arg(userID);
            query.exec(sql);
        }
        QMessageBox::information(this, tr("information"), tr("Change information successful!"));    //提示用户修改成功
    }
    close(); //关闭修改信息窗口
}
