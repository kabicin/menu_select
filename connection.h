#ifndef CONNECTION_H
#define CONNECTION_H
#include <QtSql>
#include <QList>
#include <QDir>
class connection
{
    QSqlDatabase db;
    QSqlQuery *query;
    bool connected = false;
public:
    bool createTable();
    bool createConnection();
    QList<QVariant> findItem(int);
    QList<QVariant> findItemByChName(QString);
    bool insertItem(int, QString, QString, QString);

    bool deleteItem();
    bool editItem();
    void changeFilepath(QString);
};

#endif // CONNECTION_H
