#ifndef EDITUSERINFO_H
#define EDITUSERINFO_H

#include <QDialog>

namespace Ui {
class EditUserInfo;
}

class EditUserInfo : public QDialog
{
    Q_OBJECT

public:
    explicit EditUserInfo(QWidget *parent = nullptr);
    ~EditUserInfo();

   void setUsID(int usID);


private slots:
    void on_btnClose_clicked();

    void on_btnSubmit_clicked();

private:
    Ui::EditUserInfo *ui;
    int userID;
};

#endif // EDITUSERINFO_H
