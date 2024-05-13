#include "db.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
DB::DB(const  QString hostName,const  QString dbName,const  QString userName,const  QString userPassword,unsigned int port)
{
    qDebug() << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostName);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setPassword(userPassword);
    db.setPort(3306);
    if(!db.open())
    {
        qDebug() << "连接失败：" << db.lastError().text();
    }else
    {
        qDebug() << "连接成功";
    }
}

QString DB::queryPorfilePathByUserName(QString user)
{
    QSqlQuery q;
    QString sql = "SELECT profilePath FROM user WHERE account = ";
    sql = sql + "\'" + user + "\'";
    //SELECT name FROM user WHERE account = '2089290095';
    q.exec(sql);
    while(q.next())
    {
        qDebug() << "query PorfilePath success." <<q.value(0).toString();
        return q.value(0).toString();
    }
    return " ";
}

QString DB::queryPasswordByUserName(QString user)
{
    QSqlQuery q;
    QString sql = "SELECT password FROM user WHERE account = ";
    sql = sql + "\'" + user + "\'";
    //SELECT name FROM user WHERE account = '2089290095';
    q.exec(sql);
    while(q.next())
    {
        qDebug() << "query result : " <<q.value(0).toString();
        return q.value(0).toString();
    }
    return " ";
}

QString DB::queryNameByUserName(QString user)
{
    QSqlQuery q;
    QString sql = "SELECT name FROM user WHERE account = ";
    sql = sql + "\'" + user + "\'";

    q.exec(sql);
    while(q.next())
    {
        qDebug() << "query result : " <<q.value(0).toString();
        return q.value(0).toString();
    }
    return " ";
}

QString DB::queryNickNameByUserName(QString user)
{
    QSqlQuery q;
    QString sql = "SELECT nickName FROM user WHERE account = ";
    sql = sql + "\'" + user + "\'";

    q.exec(sql);
    while(q.next())
    {
        qDebug() << "query result : " <<q.value(0).toString();
        return q.value(0).toString();
    }
    return " ";
}

QString DB::querySignatureByUserName(QString user)
{
    QSqlQuery q;
    QString sql = "SELECT signature FROM user WHERE account = ";
    sql = sql + "\'" + user + "\'";

    q.exec(sql);
    while(q.next())
    {
        qDebug() << "query result : " <<q.value(0).toString();
        return q.value(0).toString();
    }
    return " ";
}

int DB::queryVIPTypeByUserName(QString user)
{
    QSqlQuery q;
    QString sql = "SELECT vipType FROM user WHERE account = ";
    sql = sql + "\'" + user + "\'";

    q.exec(sql);
    while(q.next())
    {
        qDebug() << "query result : " <<q.value(0).toInt();
        return q.value(0).toInt();
    }
    return -1;
}
