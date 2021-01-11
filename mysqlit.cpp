#include <QDateTime>

#include "mysqlit.h"

#define DATABASE_NAME "MyDataBase.db"

#define SQL_CREATE_TABLE_LIST "CREATE TABLE IF NOT EXISTS table_list \
    (id INTEGER PRIMARY KEY AUTOINCREMENT, name text, time_group text, trancetion_time INTEGER, create_time INTEGER, type text)"

#define SQL_INSERT_TABLE_LIST "INSERT INTO table_list \
    (name, time_group, trancetion_time, create_time, type) VALUES (\"%1\", \"%2\", \"%3\", \"%4\", \"%5\")"

#define SQL_CREATE_DATA_TABLE "CREATE TABLE IF NOT EXISTS \"data\" \
    (id INTEGER PRIMARY KEY AUTOINCREMENT, table_id INTEGER, time_group text, name_number text, name text, specification text, unit text, number REAL, price REAL, price_sum REAL, comment text)"

#define SQL_INSERT_DATA "INSERT INTO \"data\" \
    (table_id, time_group, name_number, name, specification, unit, number, price, price_sum, comment) \
    VALUES (\"%1\", \"%2\", \"%3\", \"%4\", \"%5\", \"%6\", \"%7\", \"%8\", \"%9\", \"%10\")"


mysqlit::mysqlit()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(DATABASE_NAME);
    }
    if (!database.open()) {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    } else {
        qDebug() << " to connect database.";
    }

    QSqlQuery sql_query;
    QString sql_cmd = SQL_CREATE_TABLE_LIST;
    if(!sql_query.exec(sql_cmd)) {
        qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    } else {
        qDebug() << "Table created!";
    }

    sql_cmd = SQL_CREATE_DATA_TABLE;
    if(!sql_query.exec(sql_cmd)) {
     qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    } else {
     qDebug() << "Table created!";
    }

}


int mysqlit::sqlitGetTableForMonth(QString field, QVector<stru_table> &list)
{
    QSqlQuery sql_query;

    QString sql_cmd =QString("select id, name, trancetion_time, type FROM table_list WHERE time_group = \"%1\" ORDER BY trancetion_time DESC").arg(field);
//    qDebug() << sql_cmd;
    if(!sql_query.exec(sql_cmd)) {
        qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    } else {
        while(sql_query.next()) {
            stru_table table;
            table.id = sql_query.value(0).toString();
            table.name = sql_query.value(1).toString();
//            table.trancetion_time = QDateTime::fromTime_t(sql_query.value(2).toInt()).toString("yyyy-MM-dd");
            table.trancetion_time = sql_query.value(2).toString();
            table.type = sql_query.value(3).toInt();
            list.append(table);
        }
    }

    return 0;
}


int mysqlit::sqlitGetTimeGroup(QStringList &data)
{
    QSqlQuery sql_query;

    QString sql_cmd ="select time_group from table_list GROUP BY time_group ORDER BY time_group DESC";
//    qDebug() << sql_cmd;
    if(!sql_query.exec(sql_cmd)) {
        qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    } else {
        while(sql_query.next()) {
            QString time_group = sql_query.value(0).toString();
            data.append(time_group);
        }
    }

    return 0;
}


int mysqlit::sqlitGetTableData(int id, QVector<stru_table_data> &list)
{
    QSqlQuery sql_query;

    QString sql_cmd =QString("select id, table_id, name_number, name, specification, unit, number, price, price_sum, comment FROM data WHERE table_id = %1").arg(id);
        qDebug() << "jjjjjjjjjjj" <<sql_cmd;
    if(!sql_query.exec(sql_cmd)) {
        qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    } else {
        while(sql_query.next()) {
            qDebug() << "dddddddddd" ;
            stru_table_data table;
            table.id = sql_query.value(0).toString();
            table.table_id = sql_query.value(1).toString();
            table.name_number = sql_query.value(2).toString();
            table.name = sql_query.value(3).toString();
            table.specification = sql_query.value(4).toString();
            table.unit = sql_query.value(5).toString();
            table.number = sql_query.value(6).toString();
            table.price = sql_query.value(7).toString();
            table.price_sum = sql_query.value(8).toString();
            table.comment = sql_query.value(9).toString();

            list.append(table);
        }
    }

    return 0;
}

int mysqlit::sqlitGetTableData(QString time_group, QVector<stru_table_data> &list)
{
   QSqlQuery sql_query;

   QString sql_cmd =QString("select id, table_id, name_number, name, specification, unit, number, price, price_sum, comment FROM \"data\" where time_group=%1").arg(time_group);
   //    qDebug() << sql_cmd;
   if(!sql_query.exec(sql_cmd)) {
       qDebug() << "Error: Fail to create table."<< sql_query.lastError();
   } else {
       while(sql_query.next()) {
            stru_table_data table;
            table.id = sql_query.value(0).toString();
            table.table_id = sql_query.value(1).toString();
            table.name_number = sql_query.value(2).toString();
            table.name = sql_query.value(3).toString();
            table.specification = sql_query.value(4).toString();
            table.unit = sql_query.value(5).toString();
            table.number = sql_query.value(6).toString();
            table.price = sql_query.value(7).toString();
            table.price_sum = sql_query.value(8).toString();
            table.comment = sql_query.value(9).toString();

            list.append(table);
       }
   }

   return 0;
}

int mysqlit::sqlitSetTableInfo(stru_table info)
{
    QSqlQuery sql_query;
    QString sql_cmd;

    sql_cmd = QString(SQL_INSERT_TABLE_LIST).arg(info.name).arg(info.time_group).arg(info.trancetion_time).arg(info.create_time).arg(info.type);
    qDebug() << "111111" << sql_cmd;
    if(!sql_query.exec(sql_cmd)) {
        qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    } else {
        qDebug() << "list created!";
    }

    return 0;
}

int mysqlit::sqlitGetNewestTableID(int &id)
{
     QSqlQuery sql_query;
     QString sql_cmd;

     sql_cmd = QString("select id from table_list order by create_time desc limit 0,1;");
     qDebug() << "111111" << sql_cmd;
     if(!sql_query.exec(sql_cmd)) {
         qDebug() << "Error: Fail to create table."<< sql_query.lastError();
     } else {
         while(sql_query.next()) {
            id = sql_query.value(0).toInt();
         }
     }
}

int mysqlit::sqlitInsertData(stru_table_data data)
{
    QSqlQuery sql_query;
    QString sql_cmd;

    sql_cmd = QString(SQL_INSERT_DATA).arg(data.table_id).arg(data.time_group).arg(data.name_number)
                 .arg(data.name).arg(data.specification).arg(data.unit).arg(data.number)
                 .arg(data.price).arg(data.price_sum).arg(data.comment);
    qDebug() << "111111" << sql_cmd;
    if(!sql_query.exec(sql_cmd)) {
        qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    } else {
    }
    return 0;
}

void mysqlit::sqlitGetTableRecently(QVector<stru_table> &list)
{

    QSqlQuery sql_query;

    QString sql_cmd =QString("select id, name, trancetion_time, type FROM table_list ORDER BY create_time DESC limit 0, 50");
    //    qDebug() << sql_cmd;
    if(!sql_query.exec(sql_cmd)) {
    qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    } else {
        while(sql_query.next()) {
            stru_table table;
            table.id = sql_query.value(0).toString();
            table.name = sql_query.value(1).toString();
            //            table.trancetion_time = QDateTime::fromTime_t(sql_query.value(2).toInt()).toString("yyyy-MM-dd");
            table.trancetion_time = sql_query.value(2).toString();
            table.type = sql_query.value(3).toInt();
            list.append(table);
        }
    }

    return;

}


int mysqlit::testInit()
{
    QSqlQuery sql_query;

    for(int i = 0; i < 100; i++) {
        QString sql_cmd;

        QString timeGroup;
        QDateTime time = QDateTime::currentDateTime().addSecs(-(i * 1000000));

        timeGroup = time.toString("yyyy-MM");
        qDebug() << timeGroup;

        sql_cmd = QString(SQL_INSERT_TABLE_LIST).arg(QString("name %1").arg(i)).arg(timeGroup).arg(time.toTime_t()).arg(time.toTime_t()).arg("出货");
        qDebug() << sql_cmd;
        if(!sql_query.exec(sql_cmd)) {
            qDebug() << "Error: Fail to create table."<< sql_query.lastError();
        } else {
            qDebug() << "list created!";
        }

        sql_cmd = QString(SQL_INSERT_DATA).arg(timeGroup).arg(timeGroup).arg("编号").arg("名称aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa").arg("规格aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa").arg("单位").arg("数量").arg("单价").arg("金额").arg("备注");
        qDebug() << "111111" << sql_cmd;
        if(!sql_query.exec(sql_cmd)) {
            qDebug() << "Error: Fail to create table."<< sql_query.lastError();
        } else {
            qDebug() << "list created!";
        }
    }

    return 0;
}
