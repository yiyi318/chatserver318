#include"chatserver.hpp"
#include<iostream>
#include<signal.h>
#include"chatservice.hpp"
using namespace std;


//处理服务器ctrl+c结束后，重置user的状态信息
void resetHandler(int){
	ChatService::instance()->reset();
	exit(0);	

}

int main(int argc,char **argv){
	if (argc<3)
	{
		cerr<<"command invalid! example: ./CHatServer 127.0.0.1 6000"<<endl;
		exit(-1);
	}
	char *ip=argv[1];
    uint16_t port=atoi(argv[2]);
	signal(SIGINT,resetHandler);

	EventLoop loop;//创建事件对象
	InetAddress adr(ip,port);//地址
	ChatServer server(&loop, adr,"ChatServer");//创建server对象
	server.start();//开启
	loop.loop();//
	return 0;
}
