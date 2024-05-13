#include"tool.h"
#include <QPixmap>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
void NetGetPixmap(class QNetworkAccessManager* netManager, QString url)
{
    QUrl Url;
    Url = QUrl(url);
    QNetworkRequest request;
    request.setUrl(url);
    netManager->get(request);

}

