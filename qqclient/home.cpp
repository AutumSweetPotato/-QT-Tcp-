#include "home.h"
#include "ui_home.h"
#include "user.h"
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include "contactitem.h"
#include <QMouseEvent>
#include "dialogue.h"
#include "public.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QPainter>
#include <QPainterPath>
#include <QNetworkReply>
Home::Home(QString mySelfUsername,QWidget *parent) :
    QWidget(parent),m_MouseMove(false),Account(mySelfUsername),netManage(new QNetworkAccessManager(this)),
    ui(new Ui::Home)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    InitUI();

    //QListWidgetItem * pItem = new QListWidgetItem("");
    //ui->listWidget->addItem(pItem);
    //ContactItem* c1 = new ContactItem(new User("123456","博丽灵梦","赤色杀人魔",":/resources/img/headImg.jpg","这个人很懒",VIPType::NONE));
    //ui->listWidget->setItemWidget(pItem,c1);
    //ui->listWidget->setCurrentItem(pItem);

}

Home::Home(QWidget *parent):
    QWidget(parent),m_MouseMove(false),
    ui(new Ui::Home)
{
    ui->setupUi(this);
}



Home::~Home()
{
    delete ui;
}

bool Home::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->listWidget)
    {
        if(event->type() == QEvent::Enter)
        {
            ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        }
        if(event->type() == QEvent::Leave)
        {
            ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

}

QListWidget *Home::GetListWidget()
{
    return ui->listWidget;
}

void Home::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_MouseMove = true;
        m_startPoint = event->globalPosition().toPoint();
        m_windowPoint = this->frameGeometry().topLeft();
    }
}

void Home::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_MouseMove = false;
    }
}

void Home::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << __FUNCTION__;
    if( m_MouseMove)
    {
        QPoint relativePos = event->globalPosition().toPoint() - m_startPoint;
        this->move(m_windowPoint+relativePos);
    }
}

void Home::InstallEvent()
{
    ui->listWidget->installEventFilter(this);
}



void Home::InitUI()
{

    ui->profileLab->setScaledContents(true);

    InstallEvent();
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_db= new DB(IPAddres,"user","qhs","114514");
    connect(Socket,&QTcpSocket::readyRead,this,&Home::OnReadyRead);

    connect(netManage, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    //初始化本地客户端
    m_myself = GetMyself();
    ui->nameLab->setText(m_myself->m_name);
    ui->vipLab->setScaledContents(true);
    if(m_myself->m_vipType == 1)
        ui->vipLab->setPixmap(QPixmap(":/resources/img/vip.png"));
    if(m_myself->m_vipType == 2)
    ui->vipLab->setPixmap(QPixmap(":/resources/img/svip.png"));


    ui->singleLab->setText(m_myself->m_signature);
    SelectPicFormNet(netManage,PubPorfilePath + m_myself->m_profilePath);



}

User *Home::GetMyself()
{

    QString name = m_db->queryNameByUserName(Account);
    QString nickName = m_db->queryNickNameByUserName(Account);
    QString profilePath = m_db->queryPorfilePathByUserName(Account);
    QString signature = m_db->querySignatureByUserName(Account);
    int vip = m_db->queryVIPTypeByUserName(Account);
    return new User(Account,name,nickName,profilePath,signature,vip);
}



void Home::paintEvent(QPaintEvent *event)
{
    QPainter painter1(this);
    painter1.setRenderHint(QPainter::Antialiasing, true);
    painter1.fillRect(QRect(10, 10, this->width() - 2 * 10, this->height() - 2 * 10), QBrush(Qt::white));

    QColor color(50, 50, 50, 30);
    for (int i = 0; i < 10; i++)
    {
    color.setAlpha(120 - qSqrt(i) * 40);
    painter1.setPen(color);
    // 方角阴影边框;
    //painter1.drawRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
    // 圆角阴影边框;
    painter1.drawRoundedRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2, 4, 4);
    }
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
        ui->profileLab->setPixmap(pix);
    }
}


void Home::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{

    auto * w = dynamic_cast<ContactItem*>(ui->listWidget->itemWidget(item));
    int row = 0;
    if(w)
     row = ui->listWidget->row(item);

    if(!hasDialogue)
    {
     auto dia = new Dialogue(this,row);
     dia->show();
     hasDialogue = true;
    }

}

void Home::OnReadyRead()
{
    //处理服务端发送的数据

    QJsonParseError err;
    auto jdoc = QJsonDocument::fromJson(Socket->readAll(),&err);
    if(err.error != QJsonParseError::NoError)
    {
        //数据发送有误
        return;
    }
    //有新客户端连接
    if(jdoc["type"].toString() == "hasNewClient")
    {
        QString account = jdoc["data"].toString();
        QString name = m_db->queryNameByUserName(account);
        QString nickName = m_db->queryNickNameByUserName(account);
        QString profilePath = m_db->queryPorfilePathByUserName(account);
        QString signature = m_db->querySignatureByUserName(account);
        int vip = m_db->queryVIPTypeByUserName(account);
        auto user = new User(account,name,nickName,PubPorfilePath+profilePath,signature,vip);
        m_contactList.append(new ContactItem(user));
        emit AddList(user);
        addContact();
    }
    //客户端登录时
    if(jdoc["type"].toString() == "getUserHash")
    {
        if(jdoc["data"].isArray())
        {
            for(auto i : jdoc["data"].toArray())
            {
                QString account = i.toString();
                QString name = m_db->queryNameByUserName(account);
                QString nickName = m_db->queryNickNameByUserName(account);
                QString profilePath = m_db->queryPorfilePathByUserName(account);
                QString signature = m_db->querySignatureByUserName(account);
                int vip = m_db->queryVIPTypeByUserName(account);
                auto user = new User(account,name,nickName,PubPorfilePath+profilePath,signature,vip);
                m_contactList.append(new ContactItem(user));

            }
            initContact();
        }

    }
    //有客户端断开连接
    if(jdoc["type"].toString() == "clientDisconnected")
    {
        //获取断开连接的客户端的账号
        QString account = jdoc["data"].toString();
        //清除断开连接的客户端
        deleteContact(account);
    }
    //有客户端发来消息时
    if(jdoc["type"].toString() == "sendMsg")
    {
        if(!hasDialogue)
        {
            auto dia = new Dialogue(this,0);
            dia->show();
            hasDialogue = true;
        }
        QString data = jdoc["data"].toString();
        QString source = jdoc["source"].toString();
        QString target = jdoc["target"].toString();
        emit hasMsg(data,source,target);
    }
}


void Home::addContact()
{
    auto item = new QListWidgetItem;
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,*(m_contactList.end()-1));
}

void Home::initContact()
{
    if(m_contactList.isEmpty())
    {
        return;
    }

    for(int i = 0;i < m_contactList.size();i++)
    {
        auto item = new QListWidgetItem;
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item,m_contactList.at(i));
    }

}

void Home::deleteContact(QString account)
{


    for(int i = 0; i < m_contactList.size();i++)
    {
        if(m_contactList[i]->returnUser()->m_account == account)
        {
            delete m_contactList[i];
            m_contactList.removeAt(i);
            QListWidgetItem* item = ui->listWidget->takeItem(i);
            delete item;
            break;
        }

    }
    emit UpDataList(account);



}
void Home::replyFinished(QNetworkReply *reply)
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

