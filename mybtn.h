#ifndef MYBTN_H
#define MYBTN_H

#include <QPushButton>

namespace  btnFunType{
enum BtnFunType {
    normal,
    meet_will_held,
    mr_online_check,
    mr_online_status,
    agree_meet,
    refuse_meet
};
}

using namespace btnFunType;

class MyBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit MyBtn(BtnFunType funType = normal, QWidget *parent = nullptr);

    void setMeetRoomID(int mrID);   //设置会议室ID
    void setMeetID(int mrID);   //设置会议ID
    int getMeetRoomID();    //获取会议室ID
    int getMeetD(); //获取会议ID

signals:

private slots:
   void clickFunc();

private:
//    QWidget *parent;    //父窗口指针
    int meetRoomID; //会议室ID
    int meetID; //会议ID
    BtnFunType funcType;    //记录该按钮的功能类型，当按钮被点击时，根据不同的功能类型执行不同的操作

    void cancelMeet();  //普通用户取消已成功预约的会议
    void checkMeet();   //商家用户查看自己已上线会场上的会议预约
    void confirmMeet(); //商家用户去确认普通用户提交的预约
    void agreeMeet();   //商家用户确认普通用户提交的预约时，同意了预约
    void refuseMeet();  //商家用户确认普通用户提交的预约时，拒绝了预约
};

#endif // MYBTN_H
