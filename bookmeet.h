#ifndef BOOKMEET_H
#define BOOKMEET_H

#include <QDialog>

namespace Ui {
class BookMeet;
}

class BookMeet : public QDialog
{
    Q_OBJECT

public:
    explicit BookMeet(int uID, int mrID, QWidget *parent = nullptr);
    ~BookMeet();

private slots:
    void setMeetDura(); //根据用户的输入自动计算出会议时长

    void on_btnCommit_clicked();

private:
    Ui::BookMeet *ui;
    int userID;    //用户ID
    int meetRoomID;   //会议室ID
    double meetDura;   //会议时长，以小时为单位，不能超过4小时

    void setDisplay();  //设置界面中要显示的内容
};

#endif // BOOKMEET_H
