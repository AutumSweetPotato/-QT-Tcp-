#ifndef PUBLIC_H
#define PUBLIC_H
#include <QString>
extern class QTcpSocket* Socket;   //客户端socket
extern QString IPAddres;           //IP地址
extern unsigned int Port;          //端口号
extern QString PubPorfilePath;
extern bool hasDialogue;

#define SelectPicFormNet(QNetworkAccessManager,QString) \
{\
QNetworkRequest request;\
request.setUrl(QString);\
QNetworkAccessManager->get(request);\
}

#endif // PUBLIC_H
