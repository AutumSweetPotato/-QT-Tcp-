#ifndef CONTACTITEM_H
#define CONTACTITEM_H

#include <QWidget>

class User;
class ContactItem:public QWidget
{
    Q_OBJECT
public:
    ContactItem(QString account,QString name,QString nickName,QString profilePath,QString signature,int vip,QWidget *parent = nullptr);
    ContactItem(User* user);
    User* returnUser();
private:
    User* m_user;
    void InitUi();
    void updateUserDisplay();
    class QLabel* m_profileLab;
    class QLabel* m_nicknameLab;
    class QLabel* m_nameLab;
    class QLabel* m_vipTypeLab;
    class QLabel* m_signatureLab;
    class QNetworkAccessManager* manage;
    QPixmap* m_porfile;
    bool porfileIsEmpty = true;

private slots:
    void replyFinished(class QNetworkReply *reply);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // CONTACTITEM_H
