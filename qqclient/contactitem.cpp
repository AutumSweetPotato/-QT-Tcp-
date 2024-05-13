#include "contactitem.h"
#include <QLabel>
#include "user.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "public.h"
ContactItem::ContactItem(QString account,QString name,QString nickName,QString profilePath,QString signature,int vip,QWidget *parent)
    :   m_user(new User(account,name,nickName,profilePath,signature,vip)),QWidget(parent),manage(new QNetworkAccessManager(this))
{
    InitUi();
    updateUserDisplay();
}

ContactItem::ContactItem(User* user):m_user(user),manage(new QNetworkAccessManager(this))
{
    InitUi();
    updateUserDisplay();
}

User* ContactItem::returnUser()
{
    return m_user;
}

void ContactItem::InitUi()
{


    connect(manage, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    SelectPicFormNet(manage, m_user->m_profilePath);
    this->setMinimumSize(450,90);
    this->setMaximumSize(450,90);

    m_profileLab = new QLabel;
    m_nicknameLab = new QLabel;
    m_nameLab = new QLabel;
    m_vipTypeLab = new QLabel;
    m_signatureLab = new QLabel;
    m_profileLab->setFixedSize(50,50);
    auto hlayout = new QHBoxLayout;
    hlayout->addWidget(m_nicknameLab);

    hlayout->addWidget(m_nameLab);
    hlayout->addWidget(m_vipTypeLab);
    hlayout->addStretch();


    m_nameLab->setStyleSheet("color:rgb(121,121,121)");
    m_signatureLab->setStyleSheet("color:rgb(121,121,121)");

    auto glayout = new QGridLayout(this);
    glayout->addWidget(m_profileLab,0,0,2,1);
    glayout->addLayout(hlayout,0,1);
    glayout->addWidget(m_signatureLab,1,1);

    m_profileLab->setFixedSize(60,60);
    m_profileLab->setScaledContents(true);



}

void ContactItem::updateUserDisplay()
{
    if(!m_user)
        return;

    m_nicknameLab->setText(m_user->m_nickName);
    m_nameLab->setText(m_user->m_name);
    m_signatureLab->setText(m_user->m_signature);
    QString str = "(" + m_nameLab->text() + ")";
    m_nameLab->setText(str);
    switch (m_user->m_vipType) {
    case VIPType::NONE:
        m_nicknameLab->setStyleSheet("color:rgb(0,0,0)");
        break;
    case VIPType::VIP:
        m_vipTypeLab->setPixmap(QPixmap(":/resources/img/vip.png"));
        break;
    case VIPType::SVIP:
        m_nicknameLab->setStyleSheet("color:rgb(255,0,0)");
        m_vipTypeLab->setPixmap(QPixmap(":/resources/img/svip.png"));
        break;

    }




}

void ContactItem::replyFinished(QNetworkReply *reply)
{
    qDebug() << "reply :" << reply;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data_bytes = reply->readAll();
        m_porfile =new QPixmap();
        m_porfile->loadFromData(data_bytes);

        qDebug() << "img lodding success";
        porfileIsEmpty = false;
        //disconnect(netManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    }

    reply->deleteLater();
}

void ContactItem::paintEvent(QPaintEvent *event)
{
    if(!porfileIsEmpty)
    {
        QPixmap src(*m_porfile);
        QPixmap pix(src.width(),src.height());
        pix.fill(Qt::transparent);
        QPainter painter(&pix);
        painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform); //抗锯齿效果
        QPainterPath path;
        int radius = src.height()<src.width() ? (src.height()/2):(src.width()/2);
        //int radius = src.height() / 2;
        float wbh = (float)src.width()/(float)src.height();
        if(wbh > 1)
            path.addEllipse(src.rect().center(), radius*wbh,radius);
        else
        {
            float i = (float)src.height()/(float)src.width();
            path.addEllipse(src.rect().center(), radius,radius*i);
        }
        painter.setClipPath(path);
        painter.drawPixmap(pix.rect(), src);
        m_profileLab->setPixmap(pix);
    }



}
