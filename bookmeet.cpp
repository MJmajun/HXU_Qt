#include "bookmeet.h"
#include "ui_bookmeet.h"
#include "conndb.h"
#include <QDateTime>
#include <QDebug>
#include <cmath>    //使用round()函数
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <QTableWidgetItem>

BookMeet::BookMeet(int uID, int mrID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookMeet),
    userID(uID), meetRoomID(mrID)
{
    ui->setupUi(this); 
    setDisplay();
    setWindowTitle("预约");
    //关联信号和槽，使得每当用户改变会议开始时间或会议结束时间，客户端自动得出会议时长
    connect(ui->dteStart, SIGNAL(dateTimeChanged(const QDateTime)), this, SLOT(setMeetDura()));
    connect(ui->dteEnd, SIGNAL(dateTimeChanged(const QDateTime)), this, SLOT(setMeetDura()));
}

BookMeet::~BookMeet()
{
    delete ui;
}

void BookMeet::setMeetDura()
{
    QDateTime start = ui->dteStart->dateTime();
    QDateTime end = ui->dteEnd->dateTime();
    double temp = start.secsTo(end) / 3600.0; //获取到从会议开始到会议结束的小时数
    temp = round(temp * 100) / 100; //四舍五入保留两位小数
    meetDura = temp;
    ui->labMeetDura->setText(QString::number(meetDura, 'f', 2));    //更改界面上的会议时常标签

    //当时常大于4小时或小于0时，将会议时设置未红色，否则为黑色
    QPalette pa;
    if (meetDura < 0 || meetDura > 4) {
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->labMeetDura->setPalette(pa);
    }else {
        pa.setColor(QPalette::WindowText,Qt::black);
        ui->labMeetDura->setPalette(pa);
    }
}

void BookMeet::setDisplay()
{
    ui->cbMeetDura->setChecked(true);   //会议时长默认选中
    QDateTime curDT = QDateTime::currentDateTime();

    //设置默认的会议开始时间和会议结束时间都为当前时间，且最小也是当前时间
    ui->dteStart->setDateTime(curDT);
    ui->dteEnd->setDateTime(curDT);
    ui->dteStart->setMinimumDateTime(curDT);
    ui->dteEnd->setMinimumDateTime(curDT);

    //查询并设置该会议室的最大人员容纳量
    QString sql = QString("select meetroom_num from meet_room where meetroom_id = %1").arg(meetRoomID);
    QSqlQuery query(ConnDB::db);
    query.exec(sql);

    if (!query.first()) {
        QMessageBox::warning(this, tr("warning"), tr("The meeting room was not found."));
    } else {
        ui->sbMeetNum->setMaximum(query.value(0).toInt());
        ui->sbMeetNum->setValue(query.value(0).toInt());
    }

    //将提示信息的文本颜色设置为红色
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::red);
    ui->labNote->setPalette(pa);

    //显示正在召开的会议、将要召开的会议、已经取消的会议
    ui->twMeetings->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //设置列表根据内容自动设置列宽
    QString curDTStr = curDT.toString("yyyy-MM-dd hh:mm");  //以字符串保存当前时间
    sql = QString("select meet_id, app_u_id, meet_title, meet_num, start_time, end_time, operate from meet_app "
                  "where ((pay = 0 and operate = 0) or (pay = 1 and operate = 1))"
                  " and app_r_id = %1  and end_time > '%2'").arg(meetRoomID).arg(curDTStr);
    query.exec(sql);
    while (query.next()) {
        QSqlQuery query2(ConnDB::db);
        query2.exec(QString("select us_name from user_info where us_id = %1").arg(query.value(1).toInt()));
        query2.first();

        //给tableWidget添加一行，以显示会议概览
        int rowCount = ui->twMeetings->rowCount();
        ui->twMeetings->setRowCount(rowCount+1);
        int rowIndex = ui->twMeetings->rowCount() - 1;  //新增行的索引


        //定义这一行所拥有的单元格，并初始化单元格的内容
        QTableWidgetItem *twiUser = new QTableWidgetItem(query2.value(0).toString());    //申请人
        QTableWidgetItem *twiTitle = new QTableWidgetItem(query.value(2).toString());   //会议主题
        QTableWidgetItem *twiNum = new QTableWidgetItem(query.value(3).toString()); //参会人数
        QTableWidgetItem *twiSTime = new QTableWidgetItem(query.value(4).toString());   //开始时间
        QTableWidgetItem *twiETime = new QTableWidgetItem(query.value(5).toString());   //结束时间

        //设置这一行所有单元格的文本对齐格式为上下居中和左右居中
        twiUser->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiTitle->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiSTime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiETime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        if (query.value(4).toString() <= curDTStr ) { //当该会议结束时间小于等于当前时间时（即该会议正在召开），将背景色改为红色
            twiUser->setBackgroundColor(QColor(251,174, 148));
            twiTitle->setBackgroundColor(QColor(251,174, 148));
            twiNum->setBackgroundColor(QColor(251,174, 148));
            twiSTime->setBackgroundColor(QColor(251,174, 148));
            twiETime->setBackgroundColor(QColor(251,174, 148));
        } else if (query.value(4).toString() > curDTStr ) {   //当该会议开始时间大于于当前时间时（即该会议即将召开），将背景色改为蓝色
            twiUser->setBackgroundColor(QColor(213, 243, 244));
            twiTitle->setBackgroundColor(QColor(213, 243, 244));
            twiNum->setBackgroundColor(QColor(213, 243, 244));
            twiSTime->setBackgroundColor(QColor(213, 243, 244));
            twiETime->setBackgroundColor(QColor(213, 243, 244));
        }

        if (query.value(6).toInt() == 0) {   //当operate字段值为0时（即该会议被取消），将文字变为斜体，且加上删除线
            QFont font;
            font.setStrikeOut(true);
            font.setItalic(true);
            twiUser->setFont(font);
            twiTitle->setFont(font);
            twiNum->setFont(font);
            twiSTime->setFont(font);
            twiETime->setFont(font);
        }

        //向添加的行中填写数据
        ui->twMeetings->setItem(rowIndex, 0, twiUser);  //向新增行中写入会议申请人
        ui->twMeetings->setItem(rowIndex, 1, twiTitle);  //向新增行中写入会议主题
        ui->twMeetings->setItem(rowIndex, 2, twiNum);  //向新增行中写入参会人数
        ui->twMeetings->setItem(rowIndex, 3, twiSTime);  //向新增行中写入会议开始时间
        ui->twMeetings->setItem(rowIndex, 4, twiETime);  //向新增行中写入会议结束时间

    }
    ui->twMeetings->sortItems(3, Qt::AscendingOrder);   //根据会议开始时间升序排序所有已存在的会议预约信息
}

void BookMeet::on_btnCommit_clicked()
{
    //获取用户输入的会议预约信息
    QString meetTitle = ui->leMeetTitle->text().trimmed();
    QString meetType = ui->cbMeetType->currentText().trimmed();
    int num = ui->sbMeetNum->value();
    QString startTime = ui->dteStart->dateTime().toString("yyyy-MM-dd hh:mm");
    QString endTime = ui->dteEnd->dateTime().toString("yyyy-MM-dd hh:mm");
    double sumCost = ui->labSumCost->text().trimmed().toDouble();

    //    qDebug() << meetTitle << meetType << num << startTime.toString("yyyy-MM-dd hh:mm") << endTime.toString("yyyy-MM-dd hh:mm") << sumCost;

    //确认用户输入的信息是否正确
    if (meetTitle.isEmpty()) {  //如果会议主题为空，则提醒用户填写
        QMessageBox::warning(this, tr("warning"), tr("Meet title can't be empty!"));
        ui->leMeetTitle->clear();
        ui->leMeetTitle->setFocus();
        return;
    } else if (meetDura < 0 || meetDura > 4) {  //当用户预约的时长大于4小时或小于0时，提醒用户
        QMessageBox::warning(this, tr("warning"), tr("Meetings must be longer than 0 hours and less than 4 hours"));
    } else {    //确认用户的预约与已存在的会议预约是否冲突
        QTableWidgetItem *twiS =nullptr;
        QTableWidgetItem *twiE = nullptr;
        for (int i = 0; i < ui->twMeetings->rowCount(); i++)
        {
            twiS = ui->twMeetings->item(i, 3);   //获取这一条已存在预约信息的开始时间
            if (twiS->font().strikeOut()) {  //如果这一条已存在预约已经被取消，则直接检测下一条预约信息
                continue;
            } else{
                twiE = ui->twMeetings->item(i, 4);  //获取这已存在一条预约信息的结束时间
                //当用户的预约时间与已存在的预约时间重叠或存在包含时，提醒用户
                if ((startTime >= twiS->text() && startTime <= twiE->text()) ||
                        (endTime >= twiS->text() && endTime <= twiE->text()) ||
                        (startTime <= twiS->text() && endTime >= twiE->text())) {
                    QMessageBox::warning(this, tr("waring"), tr("Your appointment conflicts with others!"));
                    return;
                }
            }
        }
    }

    QSqlQuery query(ConnDB::db);
    QString sql = QString("insert into meet_app (app_u_id, app_r_id, meet_title, meet_num, start_time, end_time, people, sum, assort_id) "
                          "value (%1, %2, '%3', %4, '%5', '%6', '', %7, 1)").arg(userID).arg(meetRoomID).arg(meetTitle).arg(num).arg(startTime).arg(endTime).arg(sumCost);
    if (query.exec(sql)) {
        QMessageBox::information(this, tr("info"), tr("预约会议成功，请等待商家确认"));
    } else {
        qDebug() << query.lastError();
    }
}
