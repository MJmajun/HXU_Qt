#pragma execution_character_set("utf-8")

#include "meetsys.h"
#include "ui_meetsys.h"
#include "lwibomeet.h"
#include "lwimeeting.h"
#include "registerdlg.h"
#include "mybtn.h"
#include "edituserinfo.h"
#include "conndb.h"
#include <QSqlQuery>    //执行sql语句
#include <QDebug>
#include <QSqlError>    //sql语句错误
#include <QMessageBox>  //警告窗口
#include <QStringList>  //存储会议室名称
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QDateTime>

MeetSys::MeetSys(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MeetSys)
{
    ui->setupUi(this);
    setWindowTitle("会议系统");
    ui->twPerCenter->expandAll();
    updatePerInfo();
    ui->swPerCenter->setCurrentIndex(0);    //默认显示个人信息界面

    //设置默认商家用户登录名与密码，方便测试程序
    ui->leBusiUserName->setText("马俊");
    ui->leBusiPwd->setText("123456");

    ui->twBusiCenter->expandAll();
    ui->swBusiPlatForm->setCurrentIndex(0); //默认显示商家登录界面
    ui->swBusiCenter->setCurrentIndex(0);   //默认显示商家信息界面

    //绑定信号与槽，使每次点击个人中心和商家中心界面左边的索引时，右边切换到对应的功能界面
    connect(ui->twBusiCenter, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(switchBusiPage(QTreeWidgetItem*)));
    connect(ui->twPerCenter, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(switchPerPage(QTreeWidgetItem*)));


}

MeetSys::~MeetSys()
{
    delete ui;
}

void MeetSys::displayMeetings()
{
    updatePerInfo();    //顺便显示个人信息

    //从数据库中查询所有会议室的名称和地址
    QString sql = QString("select meetroom_id, meetroom_name, meetroom_addr from meet_room where meetroom_status = 1");
    QSqlQuery query(ConnDB::db);
    query.exec(sql);

    if (query.first()){
        //将所有会议室到名称和地址地址放到QListWidget中去
        for (int i = 0; i < query.size(); i++, query.next()){
            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(QSize(500,60));
            if(i%2 == 0) {  //每隔一条会议室信息，将背景色设置为灰色，方便用户辨认
                item->setBackgroundColor(QColor(240, 240, 240));
            }
            ui->lwBoMeet->addItem(item);
            lwiBoMeet *boMeetItem = new lwiBoMeet(ui->lwBoMeet);
            //设置每一个列表项的用户id和会议室id
            boMeetItem->setUserID(userID);
            boMeetItem->setMeetRoomID(query.value(0).toInt());
            //设置每一个列表项的会议室名称和地址
            boMeetItem->getLabMeetRoomName()->setText(query.value(1).toString().trimmed());
            boMeetItem->getLabMeetRoomAddr()->setText(query.value(2).toString().trimmed());
            //关联item和boMeetItem，以在界面中显示自定义的boMeetItem
            ui->lwBoMeet->setItemWidget(item, boMeetItem);
        }
    }


    //显示所有已存在的会议预约信息
    QString curDTStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    sql = QString("select meet_id, meet_title, app_r_id, start_time, end_time from meet_app where pay = 1 and end_time > '%1'").arg(curDTStr);
    QSqlQuery query2(ConnDB::db);
    query.exec(sql);

    if (query.first()){ //当存在会议信息时才显示，否则不显示
        query2.exec(QString("select meetroom_name, meetroom_addr from meet_room where meetroom_id = %1").arg(query.value(2).toInt()));
        if (query2.first()) {   //查询会议室信息成功时才继续执行
            //将所有即将召开和正在召开的会议预约信息显示到QListWidget中
            for (int i = 0; i < query.size(); i++, query.next()) {
                QListWidgetItem *item = new QListWidgetItem();
                item->setSizeHint(QSize(680, 70));
                ui->lwAttMeet->addItem(item);
                lwiMeeting *meetingItem = new lwiMeeting(ui->lwAttMeet);
                meetingItem->getLabMeetID()->setText("会议号：" + query.value(0).toString());
                meetingItem->getLabMeetTitle()->setText(query.value(1).toString());
                meetingItem->getLabMRName()->setText(query2.value(0).toString());
                meetingItem->getLabMRAddr()->setText(query2.value(1).toString());
                meetingItem->getLabStarTime()->setText(query.value(3).toString());
                meetingItem->getLabEndTime()->setText(query.value(4).toString());
                ui->lwAttMeet->setItemWidget(item, meetingItem);
            }
        }
    }
}


void MeetSys::getUsID(int usID)
{
    userID = usID;
}

void MeetSys::on_btnBusiLogIn_clicked()
{
    QString userName;
    QString pwd;
    userName = ui->leBusiUserName->text().trimmed();  //获取输入的用户名
    pwd = ui->leBusiPwd->text();  //获取输入的密码

    //判断用户名和密码是否为空,若为空，则弹出窗口警告
    if (userName == "") {
        QMessageBox::warning(this, tr("Warning"), tr("User name cann't be empty!"), QMessageBox::Yes);
        ui->leBusiUserName->clear();
        ui->leBusiUserName->setFocus();
        return;
    }else if (pwd == "") {
        QMessageBox::warning(this, tr("Warning"), tr("Password cann't be empty!"), QMessageBox::Yes);
        ui->leBusiPwd->clear();
        ui->leBusiPwd->setFocus();
        return;
    }

    //查询数据库中此用户是否存在
    QString sql = QString("select company_pwd, company_id from company_info where company_name = '%1'").arg(userName);
    QSqlQuery query(ConnDB::db);
    query.exec(sql);
    if (!query.first()) {   //用户不存在
        qDebug() << query.lastError().text();
        QMessageBox::warning(this, tr("Warning"), tr("This user dosen't exist!"), QMessageBox::Yes);
        ui->leBusiUserName->clear();
        ui->leBusiUserName->setFocus();
    } else if (pwd != query.value(0).toString()) {  //用户存在但密码不正确
        QMessageBox::warning(this, tr("Warning"), tr("Incorrect password!"), QMessageBox::Yes);
        ui->leBusiPwd->clear();
        ui->leBusiPwd->setFocus();
    } else if (pwd == query.value(0).toString()) {  //用户存在且密码正确
        busiUserID = query.value(1).toInt();    //获取登录成功的用户的ID
        //准备登录成功后的部分界面显示
        updateBusiInfo();   //为后面显示商家信息做准备
        ui->swBusiPlatForm->setCurrentIndex(1); //显示商家中心界面
        ui->labOwner->setText(userName);   //将添加会场功能界面的“所输入”标签内容设为登录用户的用户名
    } else {    //其他未知错误
        QMessageBox::warning(this, tr("Warning"), tr("Unknown error!"), QMessageBox::Yes);
    }
}

void MeetSys::on_btnBusiRegi_clicked()
{
    RegisterDlg *regDlg = new RegisterDlg(2, this);
    regDlg->exec();
}

void MeetSys::on_btnEditData_clicked()
{
    EditUserInfo *editUserInfo = new EditUserInfo(this);
    editUserInfo->exec();
}

void MeetSys::switchPerPage(QTreeWidgetItem *twi)
{
    QString clicked = twi->text(0);
    //    qDebug() << "点击的项目是" << clicked;
    if (clicked == "用户信息") {
        ui->swPerCenter->setCurrentIndex(0);
        updatePerInfo();
    } else if (clicked == "召开过的会议") {
        ui->swPerCenter->setCurrentIndex(1);
        updateMeetHeld();
    } else if (clicked == "已提交预约") {
        ui->swPerCenter->setCurrentIndex(2);
        updateMeetSubmitted();
    } else if (clicked == "将开会议") {
        ui->swPerCenter->setCurrentIndex(3);
        updateMeetWillHeld();
    } else if (clicked == "正在开会议") {
        ui->swPerCenter->setCurrentIndex(4);
        updateMeetUnderWay();
    } else if (clicked == "已取消会议") {
        ui->swPerCenter->setCurrentIndex(5);
        updateMeetCancelled();
    }
}

void MeetSys::switchBusiPage(QTreeWidgetItem *twi)
{
    QString clicked = twi->text(0);
    //    qDebug() << "点击的项目是" << clicked;
    if (clicked == "企业信息") {
        ui->swBusiCenter->setCurrentIndex(0);
        updateBusiInfo();
    } else if (clicked == "添加会场") {
        ui->swBusiCenter->setCurrentIndex(1);
    } else if (clicked == "已提交会场") {
        ui->swBusiCenter->setCurrentIndex(2);
        updateMRSubmitted();
    } else if (clicked == "已上线会场") {
        ui->swBusiCenter->setCurrentIndex(3);
        updateMROnLine();
    } else if (clicked == "管理上线会场") {
        ui->swBusiCenter->setCurrentIndex(4);
        updateManaOnLineMR();
    }
}

void MeetSys::updatePerInfo()
{
    QSqlQuery query(ConnDB::db);
    QString sql = QString("select us_name, us_phone, us_email, mac, us_rtime from user_info where us_id = %1").arg(userID);
    query.exec(sql);
    if (query.first()) {
        ui->labNameData->setText(query.value(0).toString());
        ui->labPhoneData->setText(query.value(1).toString());
        ui->labEmailData->setText(query.value(2).toString());
        ui->labMacData->setText(query.value(3).toString());
        ui->labRTimeData->setText(query.value(4).toString());
    }
}

void MeetSys::updateMeetHeld()
{
    ui->twMeetHeld->clearContents();    //清空已有数据
    ui->twMeetHeld->setRowCount(0); //清空已有行

    QString curDTStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    ui->twMeetHeld->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //设置列表根据内容自动设置列宽

    //查询召开过的会议
    QSqlQuery query(ConnDB::db);
    QString sql = QString("select app_r_id, meet_title, start_time, end_time from meet_app"
                          " where app_u_id = %1 and pay = 1 and end_time < '%2'").arg(userID).arg(curDTStr);
    query.exec(sql);

    while (query.next()) {
        QSqlQuery query2(ConnDB::db);
        sql = QString("select meetroom_name, meetroom_addr from meet_room where meetroom_id = %1").arg(query.value(0).toInt());
        query2.exec(sql);
        query2.first();

        //给tableWidget添加一行，以显示会议概览
        int rowCount = ui->twMeetHeld->rowCount();
        ui->twMeetHeld->setRowCount(rowCount+1);
        int rowIndex = ui->twMeetHeld->rowCount() - 1;  //新增行的索引

        //定义这一行所拥有的单元格，并初始化单元格的内容
        QTableWidgetItem *twiMeetName = new QTableWidgetItem(query2.value(0).toString());
        QTableWidgetItem *twiMeetAddr = new QTableWidgetItem(query2.value(1).toString());
        QTableWidgetItem *twiMeetTitle = new QTableWidgetItem(query.value(1).toString());
        QTableWidgetItem *twiMeetTime = new QTableWidgetItem(query.value(2).toString() + "--" + query.value(3).toString());

        //设置这一行所有单元格的文本对齐格式为上下居中和左右居中
        twiMeetName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetAddr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetTitle->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetTime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        ui->twMeetHeld->setItem(rowIndex, 0, twiMeetName);
        ui->twMeetHeld->setItem(rowIndex, 1, twiMeetAddr);
        ui->twMeetHeld->setItem(rowIndex, 2, twiMeetTitle);
        ui->twMeetHeld->setItem(rowIndex, 5, twiMeetTime);
    }

    ui->twMeetHeld->sortItems(5, Qt::AscendingOrder);   //根据会议开始时间升序排序所有已存在的会议预约信息
}

void MeetSys::updateMeetSubmitted()
{
    ui->twMeetSubmitted->clearContents();    //清空已有数据
    ui->twMeetSubmitted->setRowCount(0); //清空已有行

    //    QString curDTStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    ui->twMeetSubmitted->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //设置列表根据内容自动设置列宽

    //查询已提交预约
    QSqlQuery query(ConnDB::db);
    QString sql = QString("select app_r_id, meet_title, start_time, end_time, pay from meet_app"
                          " where app_u_id = %1").arg(userID);
    query.exec(sql);

    while (query.next()) {
        QSqlQuery query2(ConnDB::db);
        sql = QString("select meetroom_name, meetroom_addr from meet_room where meetroom_id = %1").arg(query.value(0).toInt());
        query2.exec(sql);
        query2.first();
        //给tableWidget添加一行，以显示会议概览
        int rowCount = ui->twMeetSubmitted->rowCount();
        ui->twMeetSubmitted->setRowCount(rowCount+1);
        int rowIndex = ui->twMeetSubmitted->rowCount() - 1;  //新增行的索引

        //定义这一行所拥有的单元格，并初始化单元格的内容
        QTableWidgetItem *twiMeetName = new QTableWidgetItem(query2.value(0).toString());
        QTableWidgetItem *twiMeetAddr = new QTableWidgetItem(query2.value(1).toString());
        QTableWidgetItem *twiMeetTitle = new QTableWidgetItem(query.value(1).toString());
        QTableWidgetItem *twiMeetTime = new QTableWidgetItem(query.value(2).toString() + "--" + query.value(3).toString());
        QString status;
        if (query.value(4).toInt() == 0) { status = "待付款"; }
        else if (query.value(4).toInt() == 1) { status = "已付款"; }
        else if (query.value(4).toInt() == -1) { status = "交易失败"; }
        QTableWidgetItem *twiMeetStatus= new QTableWidgetItem(status);

        //设置这一行所有单元格的文本对齐格式为上下居中和左右居中
        twiMeetName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetAddr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetTitle->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetTime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetStatus->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        ui->twMeetSubmitted->setItem(rowIndex, 0, twiMeetName);
        ui->twMeetSubmitted->setItem(rowIndex, 1, twiMeetAddr);
        ui->twMeetSubmitted->setItem(rowIndex, 2, twiMeetTitle);
        ui->twMeetSubmitted->setItem(rowIndex, 3, twiMeetTime);
        ui->twMeetSubmitted->setItem(rowIndex, 4, twiMeetStatus);
    }
    ui->twMeetSubmitted->sortItems(3, Qt::AscendingOrder);   //根据会议开始时间升序排序所有已存在的会议预约信息
}


void MeetSys::updateMeetWillHeld()
{
    ui->twMeetWillHeld->clearContents();    //清空已有数据
    ui->twMeetWillHeld->setRowCount(0); //清空已有行

    QString curDTStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    ui->twMeetWillHeld->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //设置列表根据内容自动设置列宽

    //查询已提交预约
    QSqlQuery query(ConnDB::db);
    QString sql = QString("select app_r_id, meet_title, start_time, end_time, pay, meet_id from meet_app"
                          " where app_u_id = %1 and pay = 1 and start_time > '%2'").arg(userID).arg(curDTStr);
    query.exec(sql);
    while (query.next()) {
        QSqlQuery query2(ConnDB::db);
        sql = QString("select meetroom_name, meetroom_addr from meet_room where meetroom_id = %1").arg(query.value(0).toInt());
        query2.exec(sql);
        query2.first();
        //给tableWidget添加一行，以显示会议概览
        int rowCount = ui->twMeetWillHeld->rowCount();
        ui->twMeetWillHeld->setRowCount(rowCount+1);
        int rowIndex = ui->twMeetWillHeld->rowCount() - 1;  //新增行的索引
        //定义这一行所拥有的单元格，并初始化单元格的内容
        QTableWidgetItem *twiMeetName = new QTableWidgetItem(query2.value(0).toString());
        QTableWidgetItem *twiMeetAddr = new QTableWidgetItem(query2.value(1).toString());
        QTableWidgetItem *twiMeetTitle = new QTableWidgetItem(query.value(1).toString());
        QTableWidgetItem *twiMeetTime = new QTableWidgetItem(query.value(2).toString() + "--" + query.value(3).toString());
        MyBtn *btnCancel = new MyBtn(btnFunType::meet_will_held, this);   //定义MyBtn并设置其功能类型为meet_will_held，后面以此类推
        btnCancel->setText("取消会议");
        btnCancel->setMeetID(query.value(5).toInt());

        //设置这一行所有单元格的文本对齐格式为上下居中和左右居中
        twiMeetName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetAddr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetTitle->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetTime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        ui->twMeetWillHeld->setItem(rowIndex, 0, twiMeetName);
        ui->twMeetWillHeld->setItem(rowIndex, 1, twiMeetAddr);
        ui->twMeetWillHeld->setItem(rowIndex, 2, twiMeetTitle);
        ui->twMeetWillHeld->setItem(rowIndex, 6, twiMeetTime);
        ui->twMeetWillHeld->setCellWidget(rowIndex, 7, btnCancel);
    }
    ui->twMeetWillHeld->sortItems(3, Qt::AscendingOrder);   //根据会议开始时间升序排序所有已存在的会议预约信息
}

void MeetSys::updateMeetUnderWay()
{
    ui->twMeetUnderWay->clearContents();    //清空已有数据
    ui->twMeetUnderWay->setRowCount(0); //清空已有行

    QString curDTStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    ui->twMeetUnderWay->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //设置列表根据内容自动设置列宽

    //查询召开过的会议
    QSqlQuery query(ConnDB::db);
    QString sql = QString("select app_r_id, meet_title from meet_app where app_u_id = %1 "
                          "and pay = 1 and start_time < '%2' and end_time > '%2'").arg(userID).arg(curDTStr);
    query.exec(sql);

    while (query.next()) {
        QSqlQuery query2(ConnDB::db);
        sql = QString("select meetroom_name, meetroom_addr from meet_room where meetroom_id = %1").arg(query.value(0).toInt());
        query2.exec(sql);
        query2.first();
        //给tableWidget添加一行，以显示会议概览
        int rowCount = ui->twMeetUnderWay->rowCount();
        ui->twMeetUnderWay->setRowCount(rowCount+1);
        int rowIndex = ui->twMeetUnderWay->rowCount() - 1;  //新增行的索引

        //定义这一行所拥有的单元格，并初始化单元格的内容
        QTableWidgetItem *twiMeetName = new QTableWidgetItem(query2.value(0).toString());
        QTableWidgetItem *twiMeetAddr = new QTableWidgetItem(query2.value(1).toString());
        QTableWidgetItem *twiMeetTitle = new QTableWidgetItem(query.value(1).toString());

        //设置这一行所有单元格的文本对齐格式为上下居中和左右居中
        twiMeetName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetAddr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetTitle->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        ui->twMeetUnderWay->setItem(rowIndex, 0, twiMeetName);
        ui->twMeetUnderWay->setItem(rowIndex, 1, twiMeetAddr);
        ui->twMeetUnderWay->setItem(rowIndex, 2, twiMeetTitle);
    }
}

void MeetSys::updateMeetCancelled()
{
    ui->twMeetCancelled->clearContents();    //清空已有数据
    ui->twMeetCancelled->setRowCount(0); //清空已有行

    ui->twMeetCancelled->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //设置列表根据内容自动设置列宽

    //查询召开过的会议
    QSqlQuery query(ConnDB::db);
    QString sql = QString("select app_r_id, meet_title, start_time, end_time from meet_app"
                          " where app_u_id = %1 and operate = 0").arg(userID);
    query.exec(sql);

    while (query.next()) {
        QSqlQuery query2(ConnDB::db);
        sql = QString("select meetroom_name, meetroom_addr from meet_room where meetroom_id = %1").arg(query.value(0).toInt());
        query2.exec(sql);
        query2.first();

        //给tableWidget添加一行，以显示会议概览
        int rowCount = ui->twMeetCancelled->rowCount();
        ui->twMeetCancelled->setRowCount(rowCount+1);
        int rowIndex = ui->twMeetCancelled->rowCount() - 1;  //新增行的索引

        //定义这一行所拥有的单元格，并初始化单元格的内容
        QTableWidgetItem *twiMeetName = new QTableWidgetItem(query2.value(0).toString());
        QTableWidgetItem *twiMeetAddr = new QTableWidgetItem(query2.value(1).toString());
        QTableWidgetItem *twiMeetTitle = new QTableWidgetItem(query.value(1).toString());
        QTableWidgetItem *twiMeetTime = new QTableWidgetItem(query.value(2).toString() + "--" + query.value(3).toString());

        //设置这一行所有单元格的文本对齐格式为上下居中和左右居中
        twiMeetName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetAddr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetTitle->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetTime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        ui->twMeetCancelled->setItem(rowIndex, 0, twiMeetName);
        ui->twMeetCancelled->setItem(rowIndex, 1, twiMeetAddr);
        ui->twMeetCancelled->setItem(rowIndex, 2, twiMeetTitle);
        ui->twMeetCancelled->setItem(rowIndex, 3, twiMeetTime);
    }
    ui->twMeetCancelled->sortItems(3, Qt::AscendingOrder);   //根据会议开始时间升序排序所有已存在的会议预约信息
}

void MeetSys::updateBusiInfo()
{
    QSqlQuery query(ConnDB::db);
    QString sql = QString("select company_name, company_phone, company_email, company_rtime from company_info where company_id = %1").arg(busiUserID);
    query.exec(sql);
    if (query.first()) {
        ui->labBusiNameData->setText(query.value(0).toString());
        ui->labBusiPhoneData->setText(query.value(1).toString());
        ui->labBusiEmailData->setText(query.value(2).toString());
        ui->labBusiRTimeData->setText(query.value(3).toString());
    }
}


void MeetSys::updateMRSubmitted()
{
    ui->twMRSubmitted->clearContents();    //清空已有数据
    ui->twMRSubmitted->setRowCount(0); //清空已有行

    ui->twMRSubmitted->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //设置列表根据内容自动设置列宽

    //查询已提交预约
    QSqlQuery query(ConnDB::db);
    QString sql = QString("select meetroom_name, meetroom_addr, meetroom_num, meetroom_status from meet_room where meetroom_c_id = %1").arg(busiUserID);
    query.exec(sql);

    while (query.next()) {
        //给tableWidget添加一行，以显示会议概览
        int rowCount = ui->twMRSubmitted->rowCount();
        ui->twMRSubmitted->setRowCount(rowCount+1);
        int rowIndex = ui->twMRSubmitted->rowCount() - 1;  //新增行的索引

        //定义这一行所拥有的单元格，并初始化单元格的内容
        QTableWidgetItem *twiMRName = new QTableWidgetItem(query.value(0).toString());
        QTableWidgetItem *twiMRAddr = new QTableWidgetItem(query.value(1).toString());
        QTableWidgetItem *twiMRNum = new QTableWidgetItem(query.value(2).toString());
        QString status;
        if (query.value(3).toInt() == 0) { status = "待审核"; }
        else if (query.value(3).toInt() == 1) { status = "同意"; }
        QTableWidgetItem *twiMRStatus = new QTableWidgetItem(status);

        //设置这一行所有单元格的文本对齐格式为上下居中和左右居中
        twiMRName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMRAddr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMRNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMRStatus->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        ui->twMRSubmitted->setItem(rowIndex, 0, twiMRName);
        ui->twMRSubmitted->setItem(rowIndex, 1, twiMRAddr);
        ui->twMRSubmitted->setItem(rowIndex, 2, twiMRNum);
        ui->twMRSubmitted->setItem(rowIndex, 3, twiMRStatus);
    }
}

void MeetSys::updateMROnLine()
{
    ui->twMROnLine->clearContents();    //清空已有数据
    ui->twMROnLine->setRowCount(0); //清空已有行

    ui->twMROnLine->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //设置列表根据内容自动设置列宽

    //查询已上线会场
    QSqlQuery query(ConnDB::db);
    QString sql = QString("select meetroom_id, meetroom_name, meetroom_addr, meetroom_num "
                          "from meet_room where meetroom_status = 1 and meetroom_c_id = %1").arg(busiUserID);
    query.exec(sql);

    while (query.next()) {
        //给tableWidget添加一行，以显示会议概览
        int rowCount = ui->twMROnLine->rowCount();
        ui->twMROnLine->setRowCount(rowCount+1);
        int rowIndex = ui->twMROnLine->rowCount() - 1;  //新增行的索引

        //定义这一行所拥有的单元格，并初始化单元格的内容
        QTableWidgetItem *twiMRName = new QTableWidgetItem(query.value(1).toString());
        QTableWidgetItem *twiMRAddr = new QTableWidgetItem(query.value(2).toString());
        QTableWidgetItem *twiMRNum = new QTableWidgetItem(query.value(3).toString());
        MyBtn *btnMeetSub = new MyBtn(BtnFunType::mr_online_check, this);
        btnMeetSub->setText("查看");
        btnMeetSub->setMeetRoomID(query.value(0).toInt());

        MyBtn *btnMeetSta = new MyBtn(BtnFunType::mr_online_status, this);
        QString curDTStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
        QSqlQuery query2(ConnDB::db);   //查询该会议室的待处理预约数量
        sql = QString("select meet_id from meet_app where app_r_id = %1 and pay =0 and operate = 1 and start_time > '%2'").arg(query.value(0).toInt()).arg(curDTStr);
        query2.exec(sql);
        btnMeetSta->setText(QString::number(query2.size()));
        btnMeetSta->setMeetRoomID(query.value(0).toInt());

        //设置这一行所有单元格的文本对齐格式为上下居中和左右居中
        twiMRName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMRAddr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMRNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        ui->twMROnLine->setItem(rowIndex, 0, twiMRName);
        ui->twMROnLine->setItem(rowIndex, 1, twiMRAddr);
        ui->twMROnLine->setItem(rowIndex, 2, twiMRNum);
        ui->twMROnLine->setCellWidget(rowIndex, 3, btnMeetSub);
        ui->twMROnLine->setCellWidget(rowIndex, 4, btnMeetSta);
    }
}

void MeetSys::updateManaOnLineMR()
{

}



void MeetSys::on_btnSubmit_clicked()
{
    QString mrName = ui->leMRName->text().trimmed();
    QString mrAddr = ui->leMRAddr->text().trimmed();
    int mrNum = ui->sbMRNum->value();
    bool timeIntervalOK;
    bool baseTimeOk;
    bool basePriceOk;
    int timeInterval = ui->leTimeInterval->text().toInt(&timeIntervalOK);
    int baseTime = ui->leBaseTime->text().toInt(&baseTimeOk);
    double basePrice = ui->leBasePrice->text().toDouble(&basePriceOk);

    //判断用户输入是否正确
    if (mrName.isEmpty() || mrAddr.isEmpty()) { //会议室名或地址为空
        QMessageBox::warning(this, tr("warning"), tr("Meet room name or address can't be empty!"));
    } else if (mrNum <= 0) {    //会议室容纳人数小于0
        QMessageBox::warning(this, tr("warning"), tr("Meeting room accommodation must be greater than 0"));
    } else if (!timeIntervalOK || !baseTimeOk || !basePriceOk) {    //在下面三项中输入的不是数字
        QMessageBox::warning(this, tr("warning"), tr("Please enter number in '会议间隔时间' and '会议基本时间'and '会议基本费用'"));
    } else {    //用户输入信息全部正确
        QSqlQuery query(ConnDB::db);
        QString curDTStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString sql = QString("insert into meet_room (board_mac, meetroom_c_id, meetroom_name, meetroom_addr, "
                              "meetroom_location, now_state, meetroom_service, meetroom_intr, "
                              "meetroom_headpic, meetroom_status, meetroom_apptime, meetroom_passtime, "
                              "meetroom_price, meetroom_time_interval, meetroom_basePrice, meetroom_baseTime, meetroom_num) "
                              "values ('', %1, '%2', '%3', '', 0, '', '', '', 0, '%4', '', 0.5, %5, %6, %7, %8)")
                .arg(busiUserID).arg(mrName).arg(mrAddr).arg(curDTStr).arg(timeInterval).arg(basePrice).arg(baseTime).arg(mrNum);
        if (query.exec(sql)) {
            QMessageBox::information(this, tr("info"), tr("Add meet room Successfully!"));
        } else {
            QMessageBox::warning(this, tr("Warning"), tr("Failed to add meet room"));
        }
    }
}


