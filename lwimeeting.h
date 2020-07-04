#ifndef LWIMEETING_H
#define LWIMEETING_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class lwiMeeting;
}

class lwiMeeting : public QWidget
{
    Q_OBJECT

public:
    explicit lwiMeeting(QWidget *parent = nullptr);
    ~lwiMeeting();

    QLabel* getLabMeetID();
    QLabel* getLabMeetTitle();
    QLabel* getLabMRName();
    QLabel* getLabMRAddr();
    QLabel* getLabStarTime();
    QLabel* getLabEndTime();
private:
    Ui::lwiMeeting *ui;
};

#endif // LWIMEETING_H
