#ifndef USER_H
#define USER_H

#include <QString>
enum VIPType
{
    NONE,
    VIP,
    SVIP
};
class User
{
public:
    User(QString account,QString name,QString nickName,QString profilePath,QString signature,int vip);
    QString m_account; //账号
    QString m_name;    //名称
    QString m_nickName;//昵称
    QString m_profilePath;//头像路径
    QString m_signature;//签名
    int m_vipType;  //VIP类型
};

#endif // USER_H
