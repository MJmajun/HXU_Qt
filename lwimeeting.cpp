#include "lwimeeting.h"
#include "ui_lwimeeting.h"
#include <QPalette>

lwiMeeting::lwiMeeting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lwiMeeting)
{
    ui->setupUi(this);
    ui->widLeft->setStyleSheet("background-color:rgb(252, 248, 227);");
    ui->widMid->setStyleSheet("background-color:rgb(223, 240, 216);");
    ui->widRight->setStyleSheet("background-color:rgb(82, 185, 211);");
    ui->btnAtt->setStyleSheet("background-color:rgb(225, 225, 225);");

    ui->labStartTime->setStyleSheet("color:red;");
    ui->labEndTime->setStyleSheet("color:red;");
    ui->labTimeMid->setStyleSheet("color:red;");
}

lwiMeeting::~lwiMeeting()
{
    delete ui;
}

QLabel *lwiMeeting::getLabMeetID()
{
    return ui->labMeetID;
}

QLabel *lwiMeeting::getLabMeetTitle()
{
    return ui->labMeetTitle;
}

QLabel *lwiMeeting::getLabMRName()
{
    return ui->labMRName;
}

QLabel *lwiMeeting::getLabMRAddr()
{
    return ui->labMRAddr;
}

QLabel *lwiMeeting::getLabStarTime()
{
    return ui->labStartTime;
}

QLabel *lwiMeeting::getLabEndTime()
{
    return ui->labEndTime;
}
