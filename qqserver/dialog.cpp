#include "dialog.h"
#include "./ui_dialog.h"
#include <QSettings>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include "user.h"
#include "QMessageBox"
#include <QList>
#include <QListWidget>

void test()
{


}
Dialog::Dialog(QWidget *parent)
    : QDialog(parent),m_clientNum(0)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    resize(640,480);
    m_server = new QTcpServer;
    connect(this,&Dialog::deleteUser,this,&Dialog::OnDeleteUser);
    Init();
    UpdateList();
}

Dialog::~Dialog()
{
    delete ui;
}

//初始化配置文件
void Dialog::Init()
{
    QSettings setting("./init.ini",QSettings::IniFormat);
    m_ipAddress = setting.value("Net/IP").toString();
    m_port = setting.value("Net/PORT").toUInt();
}

void Dialog::UpdateList()
{




}

void Dialog::on_btnListen_clicked()
{

    if(!(m_server->listen(QHostAddress(m_ipAddress),m_port)))
    {
        QMessageBox::critical(this,"Critical",m_server->errorString());
        exit(-1);
    }
    ui->btnListen->setEnabled(false);
    connect(m_server,&QTcpServer::newConnection,this,[=](){

        QTcpSocket* tmpSocket = m_server->nextPendingConnection();
        m_userList.push_back(new User(m_clientNum,tmpSocket));
        connect(tmpSocket,&QTcpSocket::readyRead,this,[=](){
            QString account = tmpSocket->readAll();
            if(!account.isEmpty())
            {
                m_userList[m_clientNum]->SetAccount(account);
                m_clientNum++;
            }

        });
        connect(tmpSocket,&QTcpSocket::disconnected,this,[=](){
            for(auto item : m_userList)
            {
                if(item->Socket() == tmpSocket)
                {
                    emit deleteUser(item->m_index);
                    break;
                }

            }

        });

    });

}


void Dialog::on_btnListen_2_clicked()
{
    qDebug() << __FUNCTION__;
    qDebug() << m_clientNum;
    for(auto item : m_userList)
    {

        qDebug() <<item->m_index << item->Account() << item->Socket();

    }
    //test();
}
//删除用户
void Dialog::OnDeleteUser(int index)
{

    delete m_userList[index]->Socket();
    m_userList.removeAt(index);
    for(int i = index;i < m_userList.size();i++)
    {

        m_userList[i]->m_index -= 1;
    }
    m_clientNum--;

}

