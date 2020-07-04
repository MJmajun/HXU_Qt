#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QDialog>
#include <QTableWidget>
#include "mybtn.h"

namespace Ui {
class MyTableWidget;
}

class MyTableWidget : public QDialog
{
    Q_OBJECT

public:
    explicit MyTableWidget(QWidget *parent = nullptr);
    ~MyTableWidget();

    QTableWidget *getTableWidget(); //获取窗口中的tableWidget不见，以填写数据

private:
    Ui::MyTableWidget *ui;
    friend class MyBtn;
};

#endif // MYTABLEWIDGET_H
