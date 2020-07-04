#include "lwibomeet.h"
#include "ui_lwibomeet.h"
#include "bookmeet.h"
#include <QDebug>

lwiBoMeet::lwiBoMeet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lwiBoMeet)
{
    ui->setupUi(this);

}

lwiBoMeet::~lwiBoMeet()
{
    delete ui;
}

QLabel* lwiBoMeet::getLabMeetRoomName()
{
    return ui->labMeetRoomName;
}

QLabel *lwiBoMeet::getLabMeetRoomAddr()
{
    return ui->labMeetRoomAddr;
}

void lwiBoMeet::setUserID(int uID)
{
    userID = uID;
}

void lwiBoMeet::setMeetRoomID(int mrID)
{
    meetRoomID = mrID;
}

void lwiBoMeet::on_btnBook_clicked()
{
    BookMeet* bootMeet = new BookMeet(userID, meetRoomID, this);
    bootMeet->exec();
}
