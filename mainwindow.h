#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void displayUsInfo();

public slots:
    void getUsID(int usID);

private slots:
    void on_btnEditData_clicked();
    void switchPage(QTreeWidgetItem * twi);

private:
    Ui::MainWindow *ui;
    int userID;   //登录用户的ID
};

#endif // MAINWINDOW_H
