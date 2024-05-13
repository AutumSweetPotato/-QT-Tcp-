#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QWidget>

class QPaintEvent;
class QPainter;
class QLabel;
class QMovie;
class Chatmessage : public QWidget
{
    Q_OBJECT
public:
    explicit Chatmessage(QString myselfPorfilePath,QString otherPorfilePath,QWidget *parent = nullptr);
    enum User_Type{

        User_Me,    //自己
        User_She   //用户
    };
    void setText(QString text, QSize allSize, Chatmessage::User_Type userType);
    QSize getRealString(QString src);
    QSize fontRect(QString str);
    inline QString text() {return m_msg;}
    inline User_Type userType() {return m_userType;}


protected:
    void paintEvent(QPaintEvent *event);
signals:

private slots:
   void LreplyFinished(class QNetworkReply *reply);
    void RreplyFinished(class QNetworkReply *reply);
private:

    QString m_msg;
    QSize m_allSize;
    User_Type m_userType;
    int m_kuangWidth;
    int m_textWidth;
    int m_spaceWid;
    int m_lineHeight;
    QRect m_iconLeftRect;
    QRect m_iconRightRect;
    QRect m_sanjiaoLeftRect;
    QRect m_sanjiaoRightRect;
    QRect m_kuangLeftRect;
    QRect m_kuangRightRect;
    QRect m_textLeftRect;
    QRect m_textRightRect;
    QPixmap m_leftPixmap;
    QPixmap m_rightPixmap;
    class QNetworkAccessManager* m_lmanage;
    class QNetworkAccessManager* m_rmanage;
};

#endif // CHATMESSAGE_H
