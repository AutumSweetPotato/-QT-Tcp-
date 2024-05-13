#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_btnListen_clicked();
    void on_btnListen_2_clicked();
    void OnDeleteUser(int index);
signals:
    //删除用户信号
    void deleteUser(int index);
private:
    int m_clientNum; //客户端数量
    Ui::Dialog *ui;
    void Init();//初始化UI
    class QTcpServer* m_server;
    QString m_ipAddress;
    quint16 m_port;
    void UpdateList();
    QList<class User*>m_userList;
};
#endif // DIALOG_H
