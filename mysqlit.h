#ifndef MYSQLIT_H
#define MYSQLIT_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtDebug>
#include <QVector>

class mysqlit
{
public:
    QSqlDatabase database;

    typedef struct _stru_table {
        QString id;
        QString name;
        QString dealTime;
        QString type;
    } stru_table;

    typedef struct _stru_table_data {
        QString id;
        QString table_id;
        QString name_number;
        QString name;
        QString specification;
        QString unit;
        QString number;
        QString price;
        QString price_sum;
        QString comment;
    } stru_table_data;

public:

    mysqlit();

    int sqlitCreateTable(double tableName);
    int sqlitGetTimeGroup(QStringList &data);
    int sqlitGetTableForMonth(QString field, QVector<stru_table> &list);
    int sqlitGetTableData(QString table, QString id, QVector<stru_table_data> &list);
    int sqlitGetTableData(QString table, QVector<stru_table_data> &list);




    //test
    int testInit();
};

#endif // MYSQLIT_H
