#include "Login.h"
#include "./ui_Login.h"
#include<QMovie>
#include <QMouseEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QAction>
#include "home.h"
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include "db.h"
#include <QMessageBox>
#include "public.h"
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget),m_MouseMove(false),netManager(new QNetworkAccessManager(this))

{

    ui->setupUi(this);
    Socket = new QTcpSocket;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    InitUI();





}

Login::~Login()
{
    delete ui;
}

void Login::InitUI()
{
    ui->profileimg->setScaledContents(true);
    QSettings setting("./init.ini",QSettings::IniFormat);
     IPAddres = setting.value("Net/IP").toString();
     Port = setting.value("Net/PORT").toUInt();
     PubPorfilePath = "http://"+IPAddres+":9527"+"/";

    //qDebug() << "PORT" << Port;
     m_db= new DB(IPAddres,"user","qhs","114514");
    //安装事件过滤器
    InstallEvent();

    this->setAttribute(Qt::WA_TranslucentBackground);
    QMovie* movie = new QMovie(this);
    movie->setFileName(":/resources/img/background.gif");
    movie->start();
    ui->label_top->setMovie(movie);
    ui->username->lineEdit()->setPlaceholderText("QQ/手机/邮箱");
    ui->usernameIcon->setPixmap(QPixmap(":/resources/img/QQ2.png"));
    //设置头像
    ui->profileimg->setScaledContents(true);


    QString porfilepath = PubPorfilePath + m_db->queryPorfilePathByUserName(ui->username->currentText());
    qDebug() << "porfilepath" << porfilepath;

    connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    //QNetworkRequest request;
    //request.setUrl(porfilepath);
    //netManager->get(request);
    //查询头像
    SelectPicFormNet(netManager,porfilepath);


   // qDebug ()<< "IPAddres " << IPAddres;

    connect(Socket,&QTcpSocket::connected,this,&Login::OnConnected);
    connect(Socket,&QTcpSocket::readyRead,this,&Login::OnReadyRead);
    connect(Socket,&QTcpSocket::errorOccurred,this,[](QAbstractSocket::SocketError s){
        qDebug() << s;
    });
    Socket->connectToHost(QHostAddress(IPAddres),Port);
    ui->lineEditPassword->setText("123456");

}

void Login::InstallEvent()
{
    ui->lineEditPassword->installEventFilter(this);
    ui->softKey->installEventFilter(this);
    ui->btnRetrieve->installEventFilter(this);
    ui->username->installEventFilter(this);

}

void Login::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_MouseMove = true;
        m_startPoint = event->globalPosition().toPoint();
        m_windowPoint = this->frameGeometry().topLeft();
    }
}

void Login::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_MouseMove = false;
    }
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << __FUNCTION__;
    if( m_MouseMove)
    {
        QPoint relativePos = event->globalPosition().toPoint() - m_startPoint;
        this->move(m_windowPoint+relativePos);
    }
}

void Login::on_checkBoxAutoLogin_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->checkBoxRemember->setChecked(true);
    }
}


void Login::on_checkBoxRemember_stateChanged(int arg1)
{
    if(arg1 == Qt::Unchecked)
    {
        ui->checkBoxAutoLogin->setChecked(false);
    }
}


void Login::on_btnRetrieve_clicked()
{
    QString username = "2089290095";
    QString path = "https://accounts.qq.com/find?aquin=";
    path = path + username;
    const QUrl regUrl(QLatin1String(path.toStdString()));
    QDesktopServices::openUrl(regUrl);
}

void Login::replyFinished(QNetworkReply *reply)
{
    qDebug() << "reply :" << reply;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data_bytes = reply->readAll();
            m_porfile =new QPixmap();
            m_porfile->loadFromData(data_bytes);

        qDebug() << "img lodding success";
        porfileIsEmpty = false;
    }

    reply->deleteLater();
}

//socket连接成功时
void Login::OnConnected()
{
    qDebug() << "连接服务器成功.";
    connectedSuccess = true;
    QJsonObject obj;
    obj.insert("type","reuse");
    obj.insert("account",ui->username->currentText());
    Socket->write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
}

void Login::OnReadyRead()
{
    auto jdoc = QJsonDocument::fromJson(Socket->readAll());
    if(jdoc["type"].toString() == "reuse")
    {
        reuse = true;
    }

}

bool Login::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->lineEditPassword ||watched == ui->username)
    {

        if(event->type() == QEvent::FocusIn)
        {

            if(watched == ui->lineEditPassword)
            {
                ui->lineEditPassword->setPlaceholderText("");
                ui->passwordIcon->setPixmap(QPixmap(":/resources/img/password1.png"));
            }
            else
            {
                ui->username->lineEdit()->setPlaceholderText("");
                ui->usernameIcon->setPixmap(QPixmap(":/resources/img/QQ3.png"));
            }
        }
        if(event->type() == QEvent::FocusOut)
        {
            if(watched == ui->lineEditPassword)
            {
                ui->lineEditPassword->setPlaceholderText("密码");
                ui->passwordIcon->setPixmap(QPixmap(":/resources/img/password2.png"));
            }
            else
            {
                ui->username->lineEdit()->setPlaceholderText("QQ/手机/邮箱");
                ui->usernameIcon->setPixmap(QPixmap(":/resources/img/QQ2.png"));
            }
        }
    }
    if(watched == ui->softKey || watched == ui->btnRetrieve)
    {
        if(event->type() == QEvent::Enter)
        {
            setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        }
        if(event->type() == QEvent::Leave)
        {
            setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        }
    }

    return QWidget::eventFilter(watched,event);
}

void Login::paintEvent(QPaintEvent *event)
{



    QPixmap src;
    if(!porfileIsEmpty)
    {
        src = QPixmap(*m_porfile);
        //QPixmap src(*m_porfile);
    }
    else
    {
        src = QPixmap(":/resources/img/headImg.jpg");
    }
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
    ui->profileimg->setPixmap(pix);



    QPainter painter1(this);
    painter1.setRenderHint(QPainter::Antialiasing, true);
    painter1.fillRect(QRect(SHADOW_WIDTH, SHADOW_WIDTH, this->width() - 2 * SHADOW_WIDTH, this->height() - 2 * SHADOW_WIDTH), QBrush(Qt::white));

    QColor color(50, 50, 50, 30);
    for (int i = 0; i < SHADOW_WIDTH; i++)
    {
        color.setAlpha(120 - qSqrt(i) * 40);
        painter1.setPen(color);
        // 方角阴影边框;
           painter1.drawRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, this->width() - (SHADOW_WIDTH - i) * 2, this->height() - (SHADOW_WIDTH - i) * 2);
        // 圆角阴影边框;
        //painter1.drawRoundedRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, this->width() - (SHADOW_WIDTH - i) * 2, this->height() - (SHADOW_WIDTH - i) * 2, 4, 4);
    }
}


void Login::on_btnLogin_clicked()
{

    if(connectedSuccess)
    {

        QString username = ui->username->currentText();
        QString password = m_db->queryPasswordByUserName(username);
        QMessageBox::warning(this,"",username+":"+password);
        if(password == ui->lineEditPassword->text() && username ==ui->username->currentText())
        {
        if(reuse)
        {
                QMessageBox::warning(this,"警告","该用户已经登录");
                return;
        }
        qDebug() << "登陆成功";

        //发送登陆信息给服务器
        QJsonObject obj;
        obj.insert("type","login");
        obj.insert("account",username);
        Socket->write(QJsonDocument(obj).toJson(QJsonDocument::Compact));
        //connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
        disconnect(Socket,&QTcpSocket::readyRead,this,&Login::OnReadyRead);
        Home* home = new Home(username);
        home->show();
        this->close();
        delete m_db;
        }
        else
        {
            QMessageBox::warning(this,"警告","密码为空或密码不正确");
        }
    }
    else
    {
        QMessageBox::warning(this,"警告","登陆失败，无法连接网络...");
    }





}

//账号改变时
void Login::on_username_currentTextChanged(const QString &arg1)
{
    porfileIsEmpty = true;

    QString str = m_db->queryPorfilePathByUserName(arg1);
    if(str == ' ')
    {

    }else{
        delete m_porfile;

        SelectPicFormNet(netManager,PubPorfilePath + str);
    }
    reuse = false;
    QJsonObject obj;
    obj.insert("type","reuse");
    obj.insert("account",arg1);
    Socket->write(QJsonDocument(obj).toJson(QJsonDocument::Compact));

}

