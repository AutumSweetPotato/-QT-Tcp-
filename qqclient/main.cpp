#include "Login.h"
#include <QApplication>
#include "contactitem.h"
#include "user.h"
#include "home.h"
#include "dialogue.h"
int main(int argc, char *argv[])
{
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);
    QApplication a(argc, argv);
    Login w;
    w.show();
    //ContactItem c(new User("123456","博丽灵梦","赤色杀人魔",":/resources/img/headImg.jpg","这个人很懒",VIPType::NONE));
    //c.show();
    //Home h;
    //h.show();
    //Dialogue d;
    //d.show();

    return a.exec();
}
