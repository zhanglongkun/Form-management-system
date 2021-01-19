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
        QString time_group;
        QString trancetion_time;
        long create_time;
        int type;
    } stru_table;

    typedef struct _stru_table_data {
        QString id;
        QString table_id;
        QString time_group;
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

    int sqlitGetTimeGroup(QStringList &data);
    int sqlitGetTableForMonth(QString field, QVector<stru_table> &list);
    int sqlitGetTableData(int id, QVector<stru_table_data> &list);
    int sqlitGetTableData(QString time_group, QVector<stru_table_data> &list);
    int sqlitSetTableInfo(stru_table info);
    int sqlitGetNewestTableID(int &id);
    int sqlitInsertData(stru_table_data data);
    int sqlitUpdateData(stru_table_data data);
    int sqlitDeleteData(QString id);
    void sqlitGetTableRecently(QVector<stru_table> &list);
    void sqlitGetTableListForGroup(QString group, QVector<stru_table> &list);


    //test
    int testInit();
};

#endif // MYSQLIT_H
