#include "connection.h"
#include <QDebug>
#include <QList>

bool connection::createTable(){
    return query->exec("create table food "
               "(foodid integer primary key, "
               "foodnameeng varchar(30), "
               "foodnamech varchar(10), "
               "price varchar(20))");
}

bool connection::createConnection(){
    if(!connected){
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QDir::currentPath() + "/database.db");
        qDebug() << QDir::currentPath() + "/database.db";
        query = new QSqlQuery(db);

        if(!db.open()){
            qDebug() << "[Database] Failed to open.";
            return false;
        }
        else{
            qDebug() << "[Database] Successfully connected.";
            createTable(); // by default, if table in database.db does not exist, it will create the file and or table.
            connected = true;
        }
    }
    // return true if the connection is already created or if it was just connected.
    return true;
}

QList<QVariant> connection::findItem(int num){
    QList<QVariant> values;
    query->prepare("select foodnameeng, foodnamech, price from food where foodid = ?");
    query->bindValue(0, num);
    if(!query->exec()){
        // error
        return values;
    }
    query->next();
    values.append(query->value(0));
    values.append(query->value(1));
    values.append(query->value(2));

    return values;
}

QList<QVariant> connection::findItemByChName(QString name){
    QList<QVariant> values;
    query->prepare("select foodid, foodnameeng, price from food where foodnamech = ?");
    query->bindValue(0, name);
    if(!query->exec()){
        // error
        return values;
    }
    query->next();
    values.append(query->value(0));
    values.append(query->value(1));
    values.append(query->value(2));

    return values;
}

bool connection::insertItem(int num, QString foodnameeng, QString foodnamech, QString price){

    query->prepare("insert into food values(:foodid, :foodnameeng, :foodnamech, :foodprice)");
    query->bindValue(":foodid", num);
    query->bindValue(":foodnameeng", foodnameeng);
    query->bindValue(":foodnamech", foodnamech);
    query->bindValue(":foodprice", price);
    return query->exec();
}
