#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include<muduo/net/TcpConnection.h>
#include<unordered_map>
#include<functional>
#include<mutex>
using namespace std;
using namespace muduo;
using namespace muduo::net;
#include"groupmodel.hpp"
#include"friendmodel.hpp"
#include"usermodel.hpp"
#include"offlinemessagemodel.hpp"
#include"json.hpp"
#include"redis.hpp"
using json=nlohmann::json;

//事件回调
using MsgHandler=std::function<void(const TcpConnectionPtr &conn,json &js,Timestamp)>;
//聊天服务器业务类

class ChatService{
public:
	static ChatService* instance();//获取单例对象的接口>函数

	//处理登录业务
	void login(const TcpConnectionPtr &conn,json &js,Timestamp time);
	//处理注册业务
	void reg(const TcpConnectionPtr &conn,json &js,Timestamp time);
	//一对一聊天业务
	void oneChat(const TcpConnectionPtr &conn,json &js,Timestamp time);
	//获取消息对应的处理器
	MsgHandler getHandler(int msgid);
	//处理客户端一场退出
	void clientCloseException(const TcpConnectionPtr &conn);
       
	//服务器异常
	void reset();
	//添加好友
	void addFriend(const TcpConnectionPtr &conn,json &js,Timestamp time);
	//增加群组
	void createGroup(const TcpConnectionPtr &conn,json &js,Timestamp time);
	//加入群组业务
	void addGroup(const TcpConnectionPtr &conn,json &js,Timestamp time);
	//群组对话
	void groupChat(const TcpConnectionPtr &conn,json &js,Timestamp time);

	void loginout(const TcpConnectionPtr &conn,json &js,Timestamp time);
	
	void handleRedisSubscribeMessage(int userid,string msg);
private:
	//构造函数私有化
	ChatService();
	//储存消息id和其对象的业务处理方法
	unordered_map<int,MsgHandler>_msgHandlerMap;

	//定义互斥锁，保证_userConn的线程安全
	mutex _connMutex;

	//数据操作类对象
	UserModel _userModel;

	//存储在线用户的通信连接
	unordered_map<int,TcpConnectionPtr> _userConnMap;
	OfflineMessage _offlineMsgModel;
	FriendModel _friendModel;
	//创建群组对象
	GroupModel _groupModel;
	Redis _redis;
};


#endif
