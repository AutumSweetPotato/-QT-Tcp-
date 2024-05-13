#include "chatmessage.h"
#include <QFontMetrics>
#include <QPaintEvent>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QLabel>
#include <QDebug>
#include <QPainterPath>
#include "public.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
Chatmessage::Chatmessage(QString rightPorfilePath,QString leftPorfilePath,QWidget *parent)
    : QWidget{parent},m_lmanage(new QNetworkAccessManager(this)),m_rmanage(new QNetworkAccessManager(this))

{
    QFont te_font = this->font();
    te_font.setFamily("MicrosoftYaHei");
    te_font.setPointSize(13);
    this->setFont(te_font);
    connect(m_lmanage, SIGNAL(finished(QNetworkReply*)), this, SLOT(LreplyFinished(QNetworkReply*)));
    connect(m_rmanage, SIGNAL(finished(QNetworkReply*)), this, SLOT(RreplyFinished(QNetworkReply*)));
    m_leftPixmap = QPixmap(":/resources/img/headImg.jp");
    m_rightPixmap = QPixmap(":/resources/img/headImg.jp");
    SelectPicFormNet(m_rmanage,rightPorfilePath);
    SelectPicFormNet(m_lmanage,leftPorfilePath);

}
void Chatmessage::LreplyFinished(QNetworkReply *reply)
{
    qDebug() << "reply :" << reply;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data_bytes = reply->readAll();
        m_leftPixmap.loadFromData(data_bytes);
        qDebug() << "img lodding success";
    }

    reply->deleteLater();
}
void Chatmessage::RreplyFinished(QNetworkReply *reply)
{
    qDebug() << "reply :" << reply;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data_bytes = reply->readAll();

        m_rightPixmap.loadFromData(data_bytes);
        qDebug() << "img lodding success";
    }

    reply->deleteLater();
}
void Chatmessage::setText(QString text, QSize allSize, User_Type userType)
{
    m_msg = text;
    m_userType = userType;
    m_allSize = allSize;
    this->update();
}

QSize Chatmessage::getRealString(QString src)
{
    QFontMetricsF fm(this->font());
    m_lineHeight = fm.lineSpacing();
    int nCount = src.count("\n");
    qDebug() << nCount;
    int nMaxWidth = 0;
    if(nCount == 0) {
        nMaxWidth = fm.horizontalAdvance(src);
        qDebug() <<"nMaxWidth"<< nMaxWidth;
        QString value = src;
        if(nMaxWidth > m_textWidth) {
            nMaxWidth = m_textWidth;
            int size = m_textWidth / fm.horizontalAdvance(" ");
            int num = fm.horizontalAdvance(value) / m_textWidth;
            num = ( fm.horizontalAdvance(value) ) / m_textWidth;
            nCount += num;
            QString temp = "";
            for(int i = 0; i < num; i++) {
                temp += value.mid(i*size, (i+1)*size) + "\n";
            }
            src.replace(value, temp);
        }
    } else {
        for(int i = 0; i < (nCount + 1); i++) {
            QString value = src.split("\n").at(i);
            nMaxWidth = fm.horizontalAdvance(value) > nMaxWidth ? fm.horizontalAdvance(value) : nMaxWidth;
            if(fm.horizontalAdvance(value) > m_textWidth) {
                nMaxWidth = m_textWidth;
                int size = m_textWidth / fm.horizontalAdvance(" ");
                int num = fm.horizontalAdvance(value) / m_textWidth;
                num = ((i+num)*fm.horizontalAdvance(" ") + fm.horizontalAdvance(value)) / m_textWidth;
                nCount += num;
                QString temp = "";
                for(int i = 0; i < num; i++) {
                    temp += value.mid(i*size, (i+1)*size) + "\n";
                }
                src.replace(value, temp);
            }
        }
    }
    return QSize(nMaxWidth+m_spaceWid, (nCount + 1) * m_lineHeight+2*m_lineHeight);
}

QSize Chatmessage::fontRect(QString str)
{
    m_msg = str;
    int minHei = 30;
    int iconWH = 40;
    int iconSpaceW = 20;
    int iconRectW = 5;
    int iconTMPH = 10;
    int sanJiaoW = 6;
    int kuangTMP = 20;
    int textSpaceRect = 12;
    m_kuangWidth = this->width() - kuangTMP - 2*(iconWH+iconSpaceW+iconRectW);
    qDebug() << "this width :" << this->width();
    qDebug() << "m_kuangWidth :" <<m_kuangWidth;
    m_textWidth = m_kuangWidth - 2*textSpaceRect;
    qDebug() << "m_textWidth : " << m_textWidth;
    m_spaceWid = this->width() - m_textWidth;
    m_iconLeftRect = QRect(iconSpaceW, iconTMPH, iconWH, iconWH);
    m_iconRightRect = QRect(this->width() - iconSpaceW - iconWH, iconTMPH, iconWH, iconWH);

    QSize size = getRealString(m_msg); // 整个的size

    qDebug() << "fontRect Size:" << size;
    int hei = size.height() < minHei ? minHei : size.height();

    m_sanjiaoLeftRect = QRect(iconWH+iconSpaceW+iconRectW, m_lineHeight/2, sanJiaoW, hei - m_lineHeight);
    m_sanjiaoRightRect = QRect(this->width() - iconRectW - iconWH - iconSpaceW - sanJiaoW, m_lineHeight/2, sanJiaoW, hei - m_lineHeight);

    if(size.width() < (m_textWidth+m_spaceWid)) {
        m_kuangLeftRect.setRect(m_sanjiaoLeftRect.x()+m_sanjiaoLeftRect.width(), m_lineHeight/4*3, size.width()-m_spaceWid+2*textSpaceRect, hei-m_lineHeight);
        m_kuangRightRect.setRect(this->width() - size.width() + m_spaceWid - 2*textSpaceRect - iconWH - iconSpaceW - iconRectW - sanJiaoW,
                                 m_lineHeight/4*3, size.width()-m_spaceWid+2*textSpaceRect, hei-m_lineHeight);
    } else {
        m_kuangLeftRect.setRect(m_sanjiaoLeftRect.x()+m_sanjiaoLeftRect.width(), m_lineHeight/4*3, m_kuangWidth, hei-m_lineHeight);
        m_kuangRightRect.setRect(iconWH + kuangTMP + iconSpaceW + iconRectW - sanJiaoW, m_lineHeight/4*3, m_kuangWidth, hei-m_lineHeight);
    }
    m_textLeftRect.setRect(m_kuangLeftRect.x()+textSpaceRect,m_kuangLeftRect.y()+iconTMPH,
                           m_kuangLeftRect.width()-2*textSpaceRect,m_kuangLeftRect.height()-2*iconTMPH);
    m_textRightRect.setRect(m_kuangRightRect.x()+textSpaceRect,m_kuangRightRect.y()+iconTMPH,
                            m_kuangRightRect.width()-2*textSpaceRect,m_kuangRightRect.height()-2*iconTMPH);

    return QSize(size.width(), hei);
}

void Chatmessage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//消锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));

    if(m_userType == User_Type::User_She) { // 用户
        //头像
        //        painter.drawRoundedRect(m_iconLeftRect,m_iconLeftRect.width(),m_iconLeftRect.height());
        QPainterPath path;
        path.addEllipse(m_iconLeftRect);
        painter.setClipPath(path);
        painter.drawPixmap(m_iconLeftRect, m_leftPixmap);
        path.addRect(0,0,9999,9999);
        painter.setClipPath(path);
        //框加边
        QColor col_KuangB(229, 229, 229);
        painter.setBrush(QBrush(col_KuangB));
        painter.drawRoundedRect(m_kuangLeftRect.x()-1,m_kuangLeftRect.y()-1,m_kuangLeftRect.width()+2,m_kuangLeftRect.height()+2,4,4);
        //框
        QColor col_Kuang(229, 229, 229);
        painter.setBrush(QBrush(col_Kuang));
        painter.drawRoundedRect(m_kuangLeftRect,4,4);

        //三角
        QPointF points[3] = {
            QPointF(m_sanjiaoLeftRect.x(), 30),
            QPointF(m_sanjiaoLeftRect.x()+m_sanjiaoLeftRect.width(), 25),
            QPointF(m_sanjiaoLeftRect.x()+m_sanjiaoLeftRect.width(), 35),
        };
        QPen pen;
        pen.setColor(col_Kuang);
        painter.setPen(pen);
        painter.drawPolygon(points, 3);

        //三角加边
        QPen penSanJiaoBian;
        penSanJiaoBian.setColor(col_KuangB);
        painter.setPen(penSanJiaoBian);
        painter.drawLine(QPointF(m_sanjiaoLeftRect.x() - 1, 30), QPointF(m_sanjiaoLeftRect.x()+m_sanjiaoLeftRect.width(), 24));
        painter.drawLine(QPointF(m_sanjiaoLeftRect.x() - 1, 30), QPointF(m_sanjiaoLeftRect.x()+m_sanjiaoLeftRect.width(), 36));

        //内容
        QPen penText;
        penText.setColor(QColor(0,0,0));
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        //m_textLeftRect.setWidth(m_textLeftRect.x()+ 1);
        painter.drawText(m_textLeftRect, m_msg,option);

    }  else if(m_userType == User_Type::User_Me) { // 自己
        //头像
        //        painter.drawRoundedRect(m_iconRightRect,m_iconRightRect.width(),m_iconRightRect.height());
        QPainterPath path;
        path.addEllipse(m_iconRightRect);
        painter.setClipPath(path);
        painter.drawPixmap(m_iconRightRect, m_rightPixmap);
        path.addRect(0,0,99999,99999);

        painter.setClipPath(path);
        //框`
        QColor col_Kuang(18, 183, 245);
        painter.setBrush(QBrush(col_Kuang));
        painter.drawRoundedRect(m_kuangRightRect,4,4);

        //三角
        QPointF points[3] = {
            QPointF(m_sanjiaoRightRect.x()+m_sanjiaoRightRect.width(), 30),
            QPointF(m_sanjiaoRightRect.x(), 25),
            QPointF(m_sanjiaoRightRect.x(), 35),
        };
        QPen pen;
        pen.setColor(col_Kuang);
        painter.setPen(pen);
        painter.drawPolygon(points, 3);

        //内容
        QPen penText;
        penText.setColor(Qt::white);
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        //m_textRightRect.setWidth(m_textRightRect.x()+ 1);
        painter.drawText(m_textRightRect,m_msg,option);

    }
}
