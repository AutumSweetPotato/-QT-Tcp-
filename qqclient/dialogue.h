#ifndef DIALOGUE_H
#define DIALOGUE_H
#define SHADOW_WIDTH 10
#include <QWidget>
#include "chatmessage.h"
namespace Ui {
class Dialogue;
}

class Dialogue : public QWidget
{
    Q_OBJECT

public:
    explicit Dialogue(class Home* home,int row,QWidget *parent = nullptr);
    ~Dialogue();

private:
    Ui::Dialogue *ui;
    class Home* m_home;

    void InitListWidget();
    QList<class ContactItem*>m_list;
    void copyList();
    void InitUI();
    void dealMessage( Chatmessage *messageW, class QListWidgetItem *item, QString text,  Chatmessage::User_Type type);
    QPoint m_startPoint;
    QPoint m_windowPoint;
    bool m_MouseMove;
    void InstallEvent();
    int m_initRow;
    class DB* m_db;
    class QTimer* timer;
protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_btn_max_clicked();
    void OnBtnSend();
    // QObject interface
    void on_btnClose_clicked();
    void OnUpDataList(QString str);
    void OnAddList(class User* user);
    void OnhasMsg(QString data,QString source,QString target);
    void on_m_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);


public:
    bool eventFilter(QObject *watched, QEvent *event);

    // QWidget interface

};

#endif // DIALOGUE_H
