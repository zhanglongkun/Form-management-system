#include <QDateTime>

#include "mysqlit.h"

#define DATABASE_NAME "MyDataBase.db"

#define SQL_CREATE_TABLE_LIST "CREATE TABLE IF NOT EXISTS table_list \
    (id INTEGER PRIMARY KEY AUTOINCREMENT, name text, time_group text, trancetion_time INTEGER, create_time INTEGER, type text)"

#define SQL_INSERT_TABLE_LIST "INSERT INTO table_list \
    (name, time_group, trancetion_time, create_time, type) VALUES (\"%1\", \"%2\", \"%3\", \"%4\", \"%5\")"

#define SQL_CREATE_TABLE "CREATE TABLE IF NOT EXISTS \"table_%1\" \
    (id INTEGER PRIMARY KEY AUTOINCREMENT, table_id INTEGER, name_number text, name text, specification text, unit text, number REAL, price REAL, price_sum REAL, comment text)"

#define SQL_INSERT_DATA "INSERT INTO \"table_%1\" \
    (table_id, name_number, name, specification, unit, number, price, price_sum, comment) \
    VALUES (\"%2\", \"%3\", \"%4\", \"%5\", \"%6\", \"%7\", \"%8\", \"%9\", \"%10\")"


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
}


int mysqlit::sqlitCreateTable(double tableName)
{
    QSqlQuery sql_query;

    QString sql_cmd = QString(SQL_CREATE_TABLE).arg(tableName);

    if(!sql_query.exec(sql_cmd)) {
        qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    } else {
        qDebug() << "Table created!";
    }

    return 0;
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
            table.dealTime = QDateTime::fromTime_t(sql_query.value(2).toInt()).toString("yyyy-MM-dd");
            table.type = sql_query.value(3).toString();
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


int mysqlit::sqlitGetTableData(QString table, QString id, QVector<stru_table_data> &list)
{
    QSqlQuery sql_query;

    QString sql_cmd =QString("select id, table_id, name_number, name, specification, unit, number, price, price_sum, comment FROM \"table_%1\" WHERE id = \"%2\"").arg(table).arg(id);
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

int mysqlit::sqlitGetTableData(QString table, QVector<stru_table_data> &list)
{
   QSqlQuery sql_query;

   QString sql_cmd =QString("select id, table_id, name_number, name, specification, unit, number, price, price_sum, comment FROM \"table_%1\"").arg(table);
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


        sql_cmd = QString(SQL_CREATE_TABLE).arg(timeGroup);
        qDebug() << "111111" << sql_cmd;
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
