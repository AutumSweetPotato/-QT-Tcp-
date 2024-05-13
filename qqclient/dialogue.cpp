#include "dialogue.h"
#include "ui_dialogue.h"
#include "home.h"
#include <QListWidget>
#include <QDebug>
#include <QHBoxLayout>
#include <QFile>
#include "contactitem.h"
#include "user.h"
#include <QMouseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTcpSocket>
#include"public.h"
#include <QMessageBox>
#include "db.h"
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include "public.h"
Dialogue::Dialogue(class Home* home,int row,QWidget *parent) :
    QWidget(parent),m_home(home),m_MouseMove(false),m_initRow(row),
    ui(new Ui::Dialogue)
{
    ui->setupUi(this);
    InitUI();
    InstallEvent();
    m_db= new DB(IPAddres,"user","qhs","114514");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor("#444444"));
    //设置阴影圆角
    shadow->setBlurRadius(30);
    //给嵌套QWidget设置阴影
    ui->widget_4->setGraphicsEffect(shadow);

    timer = new QTimer;
    timer->setInterval(100);
    connect(timer,&QTimer::timeout,this,[=](){

        update();
    });
    timer->start();

}

Dialogue::~Dialogue()
{
    qDebug() << "destoy";
    delete ui;
}

void Dialogue::InitListWidget()
{
    copyList();
    int count = m_home->GetListWidget()->count();
    for(int i = 0;i < count;i++)
    {

        QListWidgetItem* newItem = m_home->GetListWidget()->item(i)->clone();
        ui->m_listWidget->addItem(newItem);
        ui->m_listWidget->setItemWidget(newItem,m_list[i]);
    }

}

void Dialogue::copyList()
{
    for(int i = 0; i < m_home->m_contactList.size();i++)
    {
        ContactItem* c = new ContactItem(m_home->m_contactList[i]->returnUser());
        m_list.append(c);
    }
}

void Dialogue::InitUI()
{
    connect(ui->btnSend,&QPushButton::clicked,this,&Dialogue::OnBtnSend);
    connect(m_home,&Home::UpDataList,this,&Dialogue::OnUpDataList);
    connect(m_home,&Home::AddList,this,&Dialogue::OnAddList);
    connect(m_home,&Home::hasMsg,this,&Dialogue::OnhasMsg);

    InitListWidget();
    ui->m_listWidget->setCurrentItem(ui->m_listWidget->item(m_initRow));
    ui->label_nick->setText(m_home->GetMyself()->m_name);
    QFile file("./ListStyle.qss");
    QByteArray style;
    if(!file.open(QIODevice::ReadOnly))
        qDebug() << "qss加载失败";
    else
    {
        style = file.readAll();
        ui->m_listWidget->setStyleSheet(style);
    }
    auto action = new QAction(ui->lineEdit);
    action->setIcon(QIcon(":/resources/img/sousuo2.png"));
    ui->lineEdit->addAction(action,QLineEdit::LeadingPosition);

    ui->splitter->setStretchFactor(0,75);
    ui->splitter->setStretchFactor(1,25);
    ui->splitter->widget(1)->setMinimumHeight(180);
    ui->splitter->widget(1)->setMaximumHeight(250);
    ui->splitter_2->setStretchFactor(0,37);
    ui->splitter_2->setStretchFactor(1,63);
    ui->splitter_2->widget(0)->setMinimumWidth(95);
    ui->splitter_2->widget(0)->setMaximumWidth(300);
    ui->m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->widget_2->setFixedHeight(60);


}

void Dialogue::dealMessage(Chatmessage *messageW, QListWidgetItem *item, QString text, Chatmessage::User_Type type)
{
    messageW->setFixedWidth(ui->widget_3->width());
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(text, size, type);
    ui->msgListWidget->setItemWidget(item, messageW);
}

void Dialogue::InstallEvent()
{
    ui->btn_kongjian->installEventFilter(this);
    ui->msgListWidget->installEventFilter(this);
    ui->label_nick->installEventFilter(this);
}
bool Dialogue::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->btn_kongjian)
    {

        if(event->type() == QEvent::Enter)
        {
            this->setCursor(Qt::PointingHandCursor);
        }
        if(event->type() == QEvent::Leave)
        {
            this->setCursor(Qt::ArrowCursor);
        }
    }
    if(watched == ui->msgListWidget)
    {

        return 0;
        qDebug() << "666666";

    }
    if(watched == ui->label_nick)
    {
        if(event->type() == QEvent::Enter)
        {
            ui->label_nick->setStyleSheet("font: 500 14pt \"Microsoft YaHei UI\";text-decoration: underline;");
            this->setCursor(Qt::PointingHandCursor);
        }
        if(event->type() == QEvent::Leave)
        {
            ui->label_nick->setStyleSheet("font: 500 14pt \"Microsoft YaHei UI\";");
            this->setCursor(Qt::ArrowCursor);
        }
    }

    return QWidget::eventFilter(watched, event);
}


void Dialogue::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_MouseMove = true;
        m_startPoint = event->globalPosition().toPoint();
        m_windowPoint = this->frameGeometry().topLeft();
    }
}

void Dialogue::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_MouseMove = false;
    }
}

void Dialogue::mouseMoveEvent(QMouseEvent *event)
{
    if( m_MouseMove)
    {
        QPoint relativePos = event->globalPosition().toPoint() - m_startPoint;
        this->move(m_windowPoint+relativePos);
    }
}



void Dialogue::on_btn_max_clicked()
{
    if(!this->isMaximized())
    {
        this->showMaximized();
        ui->btn_max->setStyleSheet("image: url(:/resources/img/huifu.png);");
        ui->btn_max->setToolTip("恢复");
    }else
    {
        this->showNormal();
        ui->btn_max->setStyleSheet("image: url(:/resources/img/max.png);");
        ui->btn_max->setToolTip("最大化");
    }

}

//接收到消息
void Dialogue::OnBtnSend()
{
    //传送文本给其他客户端
    if(ui->inputEdit->toPlainText().isEmpty()  || ui->inputEdit->toPlainText().trimmed().isEmpty())
    {
        QMessageBox::warning(this,"警告","消息不能为空");
        return;
    }
    if(m_list.isEmpty())
    {
        QMessageBox::warning(this,"警告","当前聊天室没有人哦~~");
        return;
    }
    QJsonObject obj;
    QString sourceAccount = m_home->GetMyself()->m_account;
    QString targetAccount = dynamic_cast<ContactItem*>(ui->m_listWidget->itemWidget(ui->m_listWidget->item(m_initRow)))->returnUser()->m_account;
    qDebug() << "sourceAccount" << sourceAccount;
    qDebug() << "targetAccount" << targetAccount;
    obj.insert("type","sendMsg");
    obj.insert("data",ui->inputEdit->toPlainText());
    obj.insert("source",sourceAccount);
    obj.insert("target",targetAccount);
    Socket->write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    QString leftPorfilePath = m_db->queryPorfilePathByUserName(targetAccount);
    QString rightPorfilePath = m_db->queryPorfilePathByUserName(sourceAccount);
    Chatmessage* messageR = new Chatmessage(PubPorfilePath+rightPorfilePath,PubPorfilePath+leftPorfilePath,ui->msgListWidget->parentWidget());
    QListWidgetItem* itemR = new QListWidgetItem(ui->msgListWidget);
    dealMessage(messageR, itemR, ui->inputEdit->toPlainText(), Chatmessage::User_Me);
    ui->inputEdit->setPlainText("");

    ui->msgListWidget->scrollToBottom();
}




void Dialogue::on_btnClose_clicked()
{


    for(int i = 0;i < ui->m_listWidget->count();i++)
    {
       delete m_list[i];
       auto item = ui->m_listWidget->takeItem(i);
       delete item;
    }
    delete m_db;
    this->close();
    delete this;
    hasDialogue = false;
}

void Dialogue::OnUpDataList(QString str)
{
    for(int i = 0; i < m_list.size();i++)
    {
       if(m_list[i]->returnUser()->m_account == str)
       {
       delete m_list[i];
       m_list.removeAt(i);
       QListWidgetItem* item = ui->m_listWidget->takeItem(i);
       delete item;
       break;
       }

    }
}

void Dialogue::OnAddList(User* user)
{
    auto newUser = new User(*user);
    m_list.append(new ContactItem(newUser));
    auto item = new QListWidgetItem;
    ui->m_listWidget->addItem(item);
    ui->m_listWidget->setItemWidget(item,*(m_list.end()-1));
}

void Dialogue::OnhasMsg(QString data, QString source, QString target)
{

    for(int i = 0;i < ui->m_listWidget->count();i++)
    {
       if(dynamic_cast<ContactItem*>(ui->m_listWidget->itemWidget(ui->m_listWidget->item(i)))->returnUser()->m_account == source)
       {
       ui->m_listWidget->setCurrentItem(ui->m_listWidget->item(i));
       QString leftPorfilePath = m_db->queryPorfilePathByUserName(target);
       QString rightPorfilePath = m_db->queryPorfilePathByUserName(source);
       Chatmessage* messageL = new Chatmessage(PubPorfilePath+leftPorfilePath,PubPorfilePath+rightPorfilePath,ui->msgListWidget->parentWidget());
       QListWidgetItem* itemL = new QListWidgetItem(ui->msgListWidget);
       dealMessage(messageL, itemL, data, Chatmessage::User_She);
       ui->msgListWidget->scrollToBottom();
       break;
       }

    }



}


void Dialogue::on_m_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    auto * w =  dynamic_cast<ContactItem*>(ui->m_listWidget->itemWidget(current));
    if(w != nullptr)
    {
       //ui->label_nick->setText(w->returnUser()->m_nickName);
       m_initRow = ui->m_listWidget->row(current);
    }


}

