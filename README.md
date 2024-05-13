# 项目配置

- 将pic文件夹复制到桌面，这个是用户头像
- 使用qtCreator编译qqclient和qqserver，使用MinGW64套件位编译，选择cmake
- 将init.ini文件复制到编译后的输出目录，客户端和服务端都需要这个文件，文件里面的端口不用改变，ip地址根据自己的情况修改
- 使用user.sql生成user表，数据库是mysql
- QT记得安装mysql驱动
- 使用nginx.conf替换你nginx/conf/nginx.con,需要安装nginx