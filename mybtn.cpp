#include "mybtn.h"
#include "mytablewidget.h"
#include "conndb.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <QTableWidget>
#include <QHeaderView>
#include <QLayout>

MyBtn::MyBtn(BtnFunType funType, QWidget *parent)
    : QPushButton(parent), funcType(funType)
{
    connect(this, SIGNAL(clicked()), this, SLOT(clickFunc()));
}

void MyBtn::setMeetRoomID(int mrID)
{
    meetRoomID = mrID;
}

void MyBtn::setMeetID(int mrID)
{
    meetID = mrID;
}

int MyBtn::getMeetRoomID()
{
    return meetRoomID;
}

int MyBtn::getMeetD()
{
    return meetID;
}

void MyBtn::clickFunc()
{
    switch (funcType) {
        case meet_will_held: {
            cancelMeet();
            break;
        }
        case mr_online_check: {
            checkMeet();
            break;
        }
        case mr_online_status: {
            confirmMeet();
            break;
        }
        case agree_meet: {
            agreeMeet();
            break;
        }
        case refuse_meet: {
            refuseMeet();
            break;
        }
        case normal: {
        //什么都不做
        }
        defualt: {
            qDebug() << "MyBtn error!";
        }
    }
}

void MyBtn::cancelMeet()
{
    QMessageBox message(QMessageBox::Warning,"Information", QString("确认取消会议?"), QMessageBox::Yes|QMessageBox::No, parentWidget());
    if (message.exec() == QMessageBox::Yes)
    {
        QSqlQuery query(ConnDB::db);
        QString sql = QString("update meet_app set pay = 0, operate = 0 where meet_id = %1").arg(meetID);
        query.exec(sql);
    }
}

void MyBtn::checkMeet()
{
    MyTableWidget myTWDlg(parentWidget()); //创建一个对话框，里面有一个tableWidget显示信息
    QTableWidget *tw = myTWDlg.getTableWidget();
    tw->removeColumn(7);    //该函数只是查看已经预约成功的会议，不需要也不应该有最后一列，所以清除最后一列
    tw->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //设置列表根据内容自动设置列宽

    QString curDTStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");

    QSqlQuery query(ConnDB::db);
    QString sql = QString("select app_u_id, meet_title, meet_num, start_time, end_time from meet_app "
                          "where app_r_id = %1 and pay = 1 and operate = 1 and start_time > '%2'").arg(meetRoomID).arg(curDTStr);
    query.exec(sql);
    while (query.next()) {
        QSqlQuery query2(ConnDB::db);
        sql = QString("select us_name, us_phone, us_email from user_info where us_id = %1").arg(query.value(0).toInt());
        query2.exec(sql);
        query2.first();
        //给tableWidget添加一行，以显示会议概览
        int rowCount = tw->rowCount();
        tw->setRowCount(rowCount+1);
        int rowIndex = tw->rowCount() - 1;  //新增行的索引

        //定义这一行所拥有的单元格，并初始化单元格的内容
        QTableWidgetItem *twiOrganizer = new QTableWidgetItem(query2.value(0).toString());
        QTableWidgetItem *twiPhone = new QTableWidgetItem(query2.value(1).toString());
        QTableWidgetItem *twiEmail = new QTableWidgetItem(query2.value(2).toString());
        QTableWidgetItem *twiMeetTitle = new QTableWidgetItem(query.value(1).toString());
        QTableWidgetItem *twiMeetNum = new QTableWidgetItem(query.value(2).toString());
        QTableWidgetItem *twiMeetST = new QTableWidgetItem(query.value(3).toString());
        QTableWidgetItem *twiMeetET = new QTableWidgetItem(query.value(4).toString());


        //设置这一行所有单元格的文本对齐格式为上下居中和左右居中
        twiOrganizer->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiPhone->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetTitle->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiEmail->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetST->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetET->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        tw->setItem(rowIndex, 0, twiOrganizer);
        tw->setItem(rowIndex, 1, twiPhone);
        tw->setItem(rowIndex, 2, twiEmail);
        tw->setItem(rowIndex, 3, twiMeetTitle);
        tw->setItem(rowIndex, 4, twiMeetNum);
        tw->setItem(rowIndex, 5, twiMeetST);
        tw->setItem(rowIndex, 6, twiMeetET);
    }

    tw->sortItems(5, Qt::AscendingOrder);   //根据会议开始时间升序排序所有已存在的会议预约信息
    myTWDlg.exec(); //显示会议预约信息
}

void MyBtn::confirmMeet()
{
    MyTableWidget myTWDlg(parentWidget()); //创建一个对话框，里面有一个tableWidget显示信息
    QTableWidget *tw = myTWDlg.getTableWidget();
    tw->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //设置列表根据内容自动设置列宽
    tw->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //设置列表根据内容自动设置行高

    QString curDTStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");

    QSqlQuery query(ConnDB::db);
    QString sql = QString("select app_u_id, meet_title, meet_num, start_time, end_time, meet_id from meet_app "
                          "where app_r_id = %1 and pay = 0 and operate = 1 and start_time > '%2'").arg(meetRoomID).arg(curDTStr);
    query.exec(sql);
    while (query.next()) {
        QSqlQuery query2(ConnDB::db);
        sql = QString("select us_name, us_phone, us_email from user_info where us_id = %1").arg(query.value(0).toInt());
        query2.exec(sql);
        query2.first();
        //给tableWidget添加一行，以显示会议概览
        int rowCount = tw->rowCount();
        tw->setRowCount(rowCount+1);
        int rowIndex = tw->rowCount() - 1;  //新增行的索引

        //定义这一行所拥有的单元格，并初始化单元格的内容
        QTableWidgetItem *twiOrganizer = new QTableWidgetItem(query2.value(0).toString());
        QTableWidgetItem *twiPhone = new QTableWidgetItem(query2.value(1).toString());
        QTableWidgetItem *twiEmail = new QTableWidgetItem(query2.value(2).toString());
        QTableWidgetItem *twiMeetTitle = new QTableWidgetItem(query.value(1).toString());
        QTableWidgetItem *twiMeetNum = new QTableWidgetItem(query.value(2).toString());
        QTableWidgetItem *twiMeetST = new QTableWidgetItem(query.value(3).toString());
        QTableWidgetItem *twiMeetET = new QTableWidgetItem(query.value(4).toString());
        //最后一个单元格需要两个按钮。自定义一个widget以容纳两个按钮
        QWidget *wid = new QWidget();
        MyBtn *btnAgr = new MyBtn(BtnFunType::agree_meet, tw);
        MyBtn *btnRef = new MyBtn(BtnFunType::refuse_meet, tw);
        btnAgr->setText("同意");
        btnRef->setText("拒绝");
        btnAgr->setMeetID(query.value(5).toInt());
        btnRef->setMeetID(query.value(5).toInt());
        btnAgr->setMeetRoomID(meetRoomID);
        btnRef->setMeetRoomID(meetRoomID);
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget(btnAgr);
        layout->addWidget(btnRef);
        wid->setLayout(layout);

        //设置这一行所有单元格的文本对齐格式为上下居中和左右居中
        twiOrganizer->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiPhone->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetTitle->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiEmail->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetST->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiMeetET->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        tw->setItem(rowIndex, 0, twiOrganizer);
        tw->setItem(rowIndex, 1, twiPhone);
        tw->setItem(rowIndex, 2, twiEmail);
        tw->setItem(rowIndex, 3, twiMeetTitle);
        tw->setItem(rowIndex, 4, twiMeetNum);
        tw->setItem(rowIndex, 5, twiMeetST);
        tw->setItem(rowIndex, 6, twiMeetET);
        tw->setCellWidget(rowIndex, 7, wid);
    }
    tw->sortItems(5, Qt::AscendingOrder);   //根据会议开始时间升序排序所有已存在的会议预约信息
    myTWDlg.exec(); //显示会议预约信息
}

void MyBtn::agreeMeet()
{
    QMessageBox message(QMessageBox::Warning,"Information", QString("确定同意预约?"), QMessageBox::Yes|QMessageBox::No, parentWidget());
    if (message.exec() == QMessageBox::Yes)
    {
        QSqlQuery query(ConnDB::db);
        QString sql = QString("update meet_app set pay = 1 where meet_id = %1").arg(meetID);
        query.exec(sql);
    }
}

void MyBtn::refuseMeet()
{
    QMessageBox message(QMessageBox::Warning,"Information", QString("确定拒绝预约?"), QMessageBox::Yes|QMessageBox::No, parentWidget());
    if (message.exec() == QMessageBox::Yes)
    {
        QSqlQuery query(ConnDB::db);
        QString sql = QString("update meet_app set pay = -1 where meet_id = %1").arg(meetID);
        query.exec(sql);
    }
}
