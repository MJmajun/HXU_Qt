#include "mytablewidget.h"
#include "ui_mytablewidget.h"

MyTableWidget::MyTableWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyTableWidget)
{
    ui->setupUi(this);
}

MyTableWidget::~MyTableWidget()
{
    delete ui;
}

QTableWidget *MyTableWidget::getTableWidget()
{
    return ui->tableWidget;
}
