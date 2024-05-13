#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QHash>



class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
private slots:

    void OnNewConnection();
    void OnReadyRead();
    void OnDisConnected();
private:
    void Init();
    QString m_ipAddres;
    quint16 m_port;
    QTcpServer m_server;
    QHash<QString,QTcpSocket*> m_userHash;
    void print();
signals:

};

#endif // SERVER_H
