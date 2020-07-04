#ifndef CONNDB_H
#define CONNDB_H

#include <QSqlDatabase>

class ConnDB
{

public:

    inline ConnDB(){}
    inline ~ConnDB(){}

    static QSqlDatabase db; //数据库对象，其他文件可以使用该数据库执行查询
    static bool initDB();   //初始化数据库对象db
};

#endif // CONNDB_H
