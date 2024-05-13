#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include "db.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Home;
}
QT_END_NAMESPACE
class QListWidgetItem;
class Home : public QWidget
{
    Q_OBJECT

public:
    Home(QString mySelfUsername,QWidget *parent = nullptr);
    Home(QWidget *parent = nullptr);
    ~Home();

public:
    bool eventFilter(QObject *watched, QEvent *event);
    class QListWidget* GetListWidget();
    QList<class ContactItem*>m_contactList;
    class User* GetMyself();
signals:
    void UpDataList(QString str);
    void AddList(class User* user);
    void hasMsg(QString data,QString source,QString target);
private:

    QPoint m_startPoint;
    QPoint m_windowPoint;
    bool m_MouseMove;
    class User* m_myself;
private slots:

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void OnReadyRead();
    void replyFinished(class QNetworkReply *reply);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    //安装事件过滤器
    void InstallEvent();
private:
    Ui::Home *ui;
    void InitUI();


    DB *m_db;
    QString Account;
    QPixmap* m_porfile;
    bool porfileIsEmpty = true;

    void addContact();
    void initContact();
    void deleteContact(QString account);
    class QNetworkAccessManager* netManage;
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // HOME_H
