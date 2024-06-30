# chatserver
一个基于muduo库的集群聊天服务器 基于nginx,redis,mysql

运行环境是在linux环境下

使用前，需要安装第三方库如boost,redis,nginx,mysql等

在./ChatServer318文件下 
运行指令 cmake. 然后再运行make 
然后会生成服务端可执行文件和客户端可执行文件（./bin文件下）

服务端开启指令 ./ChatServer 127.0.0.1 6000 和./ChatServer 127.0.0.1 6002
客户端开始指令 ./ChatClient 127.0.0.1 8000
