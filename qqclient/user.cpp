#include "user.h"

User::User(QString account, QString name, QString nickName, QString profilePath, QString signature, int vip)
    :m_account(account),m_name(name),m_nickName(nickName),m_profilePath(profilePath),
    m_signature(signature),m_vipType(vip)
{

}
