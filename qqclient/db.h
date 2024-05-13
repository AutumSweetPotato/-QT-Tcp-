#ifndef DB_H
#define DB_H
#include <QString>
#include <QSqlDatabase>

class DB
{
public:
    DB(const  QString hostName,const  QString dbName,const  QString userName,const  QString userPassword,unsigned int port = 3306);
private:
    QSqlDatabase db;
public:
    QString queryPorfilePathByUserName(QString user);
    QString queryPasswordByUserName(QString user);
    QString queryNameByUserName(QString user);
    QString queryNickNameByUserName(QString user);
    QString querySignatureByUserName(QString user);
    int queryVIPTypeByUserName(QString user);
};

#endif // DB_H
