#ifndef MEETSYS_H
#define MEETSYS_H

#include <QMainWindow>
#include <QTreeWidgetItem>

namespace Ui {
class MeetSys;
}

class MeetSys : public QMainWindow
{
    Q_OBJECT

public:
    explicit MeetSys(QWidget *parent = nullptr);
    ~MeetSys();

    void displayMeetings();    //显示会议室和会议预约信息

public slots:
    void getUsID(int usID);

private slots:
    void on_btnBusiLogIn_clicked();
    void on_btnBusiRegi_clicked();

    void switchBusiPage(QTreeWidgetItem *twi);  //根据个人中心中用户点击界面左部的索引，切换右边的功能界面
    void switchPerPage(QTreeWidgetItem *twi);   //根据商家平台中用户点击界面左部的索引，切换右边的功能界面

    void on_btnSubmit_clicked();
    void on_btnEditData_clicked();

private:
    Ui::MeetSys *ui;
    int userID; //普通用户ID
    int busiUserID; //商家用户ID

    //每次点击索引，都更新对应的功能界面的数据，下面是系列更新槽函数
    void updatePerInfo();
    void updateMeetHeld();
    void updateMeetSubmitted();
    void updateMeetWillHeld();
    void updateMeetUnderWay();
    void updateMeetCancelled();
    void updateBusiInfo();
    void updateMRSubmitted();
    void updateMROnLine();
    void updateManaOnLineMR();
};

#endif // MEETSYS_H
