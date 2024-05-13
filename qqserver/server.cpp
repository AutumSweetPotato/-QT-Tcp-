#include "server.h"
#include <QSettings>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
Server::Server(QObject *parent)
    : QObject{parent}
{
    Init();
}

void Server::OnNewConnection()
{
    qDebug() << "has client connected";
    while(m_server.hasPendingConnections())
    {
        QTcpSocket* socket = m_server.nextPendingConnection();
        qDebug() << socket;
        if(!socket)
        {
            qDebug() << socket->errorString();
            continue;
        }
        connect(socket,&QTcpSocket::readyRead,this,&Server::OnReadyRead);
        connect(socket,&QTcpSocket::disconnected,this,&Server::OnDisConnected);

    }


}

void Server::OnReadyRead()
{
    auto socket = dynamic_cast<QTcpSocket*>(sender());
    //接受客户端信息
    QJsonParseError err;
    auto jdoc = QJsonDocument::fromJson(socket->readAll(),&err);
    if(err.error != QJsonParseError::NoError)
    {
        //数据发送有误
        return;
    }
    if(jdoc["type"].toString() == "reuse")
    {
        if(m_userHash.find(jdoc["account"].toString()) != m_userHash.end())
        {
            QJsonObject obj;
            obj.insert("type","reuse");
            socket->write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
        }

    }
    if(jdoc["type"].toString() == "login")
    {
        QHash<QString,QTcpSocket*>::iterator it = m_userHash.begin();
        //m_userHash.insert(jdoc["account"].toString(),socket);
        //发送消息让客户端更新联系人
        //QJsonObject obj;
        //QJsonArray userList;
        //obj.insert("type","updateList");
        if(!m_userHash.isEmpty())
        {
            while(it != m_userHash.end())
            {
                QJsonObject obj;
                obj.insert("type","hasNewClient");
                obj.insert("data",jdoc["account"].toString());
                it.value()->write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
                it++;
            }
        }
        if(!m_userHash.isEmpty())
        {
            QJsonObject obj;
            QJsonArray userList;
            it = m_userHash.begin();
            while(it != m_userHash.end())
            {
                userList.append(it.key());
                it++;
            }
            obj.insert("type","getUserHash");
            obj.insert("data",userList);
            socket->write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
        }
        m_userHash.insert(jdoc["account"].toString(),socket);
    }
    if(jdoc["type"].toString() == "sendMsg")
    {
        QString data = jdoc["data"].toString();
        QString sourceAccount = jdoc["source"].toString();
        QString targetAccount = jdoc["target"].toString();
        QJsonObject obj;
        obj.insert("type","sendMsg");
        obj.insert("data",data);
        obj.insert("source",sourceAccount);
        obj.insert("target",targetAccount);
        QHash<QString,QTcpSocket*>::iterator it = m_userHash.begin();
        while(it != m_userHash.end())
        {
            if(it.key() != sourceAccount)
            it.value()->write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
            it++;
        }

    }
    print();

}

void Server::OnDisConnected()
{
    auto socket = dynamic_cast<QTcpSocket*>(sender());
    //让其他客户端删除断开的客户端
    if(m_userHash.key(socket,"no") !="no")
    {
        QHash<QString,QTcpSocket*>::iterator it = m_userHash.begin();
        while(it != m_userHash.end())
        {
            if(m_userHash.key(socket) != it.key())
            {
                QJsonObject obj;
                obj.insert("type","clientDisconnected");
                obj.insert("data",m_userHash.key(socket));
                it.value()->write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
            }
            it++;
        }
    }

    m_userHash.remove(m_userHash.key(socket));
    delete socket;
    //qDebug() << socket << "断开连接";
}

void Server::Init()
{
    //加载IP地址和端口号
    QSettings setting("./init.ini",QSettings::IniFormat);
    m_ipAddres = setting.value("Net/IP").toString();
    m_port = setting.value("Net/PORT").toUInt();

    //开启监听
    if(!(m_server.listen(QHostAddress(m_ipAddres),m_port)))
    {

        qDebug() << "服务器开启失败." << m_server.errorString();
        exit(-1);
    }
    qDebug() << "服务器开启成功.";
    connect(&m_server,&QTcpServer::newConnection,this,&Server::OnNewConnection);

}

void Server::print()
{
    QHash<QString,QTcpSocket*>::const_iterator it = m_userHash.constBegin();
    qDebug() << "-----------------";
    while(it != m_userHash.constEnd())
    {

        qDebug() << it.key() << it.value();

        it++;
    }
    qDebug() << "-----------------";
}
