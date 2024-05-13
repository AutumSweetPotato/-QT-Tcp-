#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
#define SHADOW_WIDTH 10		// 阴影边框宽度;
class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();



private:
    Ui::Widget *ui;
    bool m_MouseMove;

    //初始化UI
    void InitUI();
    //安装事件过滤器
    void InstallEvent();
    QPoint m_startPoint;
    QPoint m_windowPoint;
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_checkBoxAutoLogin_stateChanged(int arg1);
    void on_checkBoxRemember_stateChanged(int arg1);
    void on_btnRetrieve_clicked();
    void replyFinished(class QNetworkReply *reply);
    void OnConnected();
    void OnReadyRead();
    // QWidget interface



    // QObject interface


    void on_btnLogin_clicked();
    void on_username_currentTextChanged(const QString &arg1);

public:
    bool eventFilter(QObject *watched, QEvent *event);

private:

    class DB* m_db;
    class QNetworkAccessManager* netManager;
    // QWidget interface'
    QPixmap* m_porfile;
    bool porfileIsEmpty = true;
    bool connectedSuccess = false;
    bool reuse = false;

protected:
    void paintEvent(QPaintEvent *event);
};
#endif // LOGIN_H
