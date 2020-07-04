#ifndef CHOSYSDLG_H
#define CHOSYSDLG_H

#include <QDialog>

namespace Ui {
class ChoSysDlg;
}

class ChoSysDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ChoSysDlg(QWidget *parent = nullptr);
    ~ChoSysDlg();

signals:
    void postRes(int res);

private slots:

    void on_btnExit_clicked();

    void on_btnConfirm_clicked();

private:
    Ui::ChoSysDlg *ui;

};

#endif // CHOSYSDLG_H
