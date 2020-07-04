#include "conndb.h"
#include <QMessageBox>
#include <QSqlError>

QSqlDatabase ConnDB::db;

bool ConnDB::initDB()
{
    db = QSqlDatabase::addDatabase("QMYSQL");

//    db.setHostName("localhost");
//    db.setPort(3306);
//    db.setDatabaseName("mei2");
//    db.setUserName("tangjun");
//    db.setPassword("123456");

    db.setHostName("123.57.243.85");
    db.setPort(3307);
    db.setUserName("tangjun");
    db.setPassword("123456");
    db.setDatabaseName("testTangJun");

    if (!db.open()) {
        QMessageBox::warning(nullptr, QString("Warning"), db.lastError().text(), QMessageBox::Cancel);
        return false;
    }

    return true;
}
