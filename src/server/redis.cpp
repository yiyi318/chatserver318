#include"redis.hpp"
#include<iostream>
using namespace std;

Redis::Redis()
	:_publish_context(nullptr),_subcribe_context(nullptr)
{
}

Redis::~Redis(){
	if(_publish_context!=nullptr){
		redisFree(_publish_context);
	}
	if(_subcribe_context!=nullptr)
	{
		redisFree(_subcribe_context);
	
	}
}

bool Redis::connect(){
	//负责publish发布消息的上下文连接
	_publish_context=redisConnect("127.0.0.1",6379);
	if(nullptr==_publish_context)
	{
		cerr<<"connect redis failed!"<<endl;
		return false;
	}else{
        cerr<<"connect redis successed"<<endl;    
    }
	//负责subscribe订阅消息的上下文连接
	_subcribe_context=redisConnect("127.0.0.1",6379);
	if(nullptr==_subcribe_context){
		cerr<<"connect redis failed!"<<endl;
		return false;
	}
    else{
        cerr<<"connect redis really successed"<<endl;   
    }
	//在单独的小城中，监听通道上的事件，有消息给业务层进行上报
	thread t([&](){
		observer_channel_message();
	});
	t.detach();
	cout<<"connect redis-server success!"<<endl;
	return true;
}

//向redis指定的通道channel发布消息
bool Redis::publish(int channel,string message){
	redisReply*reply=(redisReply *) redisCommand(_publish_context,"PUBLISH %d %s",channel,message);
	if(nullptr==reply){
		cerr<<"publish command failed!"<<endl;
		return false;
	}
	freeReplyObject(reply);
	return true;
}


//向Redis指定的通道subscribe订阅消息
//循环堵塞的方式

bool Redis::subscribe(int channel){
	if(REDIS_ERR==redisAppendCommand(this->_subcribe_context,"SUBSCRIBE %d",channel))
	{
		cerr<<"subscribe command failed!"<<endl;
		return false;
	}else{
        cerr<<"subscribe command successed!"<<endl;
    }

	int done=0;
	while(!done){
		if(REDIS_ERR==redisBufferWrite(this->_subcribe_context,&done)){
			cerr<<"subscribe command failed!"<<endl;
			return false;
		}
	}
	return true;
}

//向redis指定的通道unsubscribe取消订阅消息
bool Redis::unsubscribe(int channel){
	if(REDIS_ERR==redisAppendCommand(this->_subcribe_context,"UNSUBSCRIBE %d",channel))
        {
                cerr<<"unsubscribe command failed!"<<endl;
                return false;
        }
    else{
         cerr<<"unsubscribe command successed!"<<endl;    
    }

        int done=0;
        while(!done){
                if(REDIS_ERR==redisBufferWrite(this->_subcribe_context,&done)){
                        cerr<<"unsubscribe command failed!"<<endl;
                        return false;
                }
        }
        return true;

}

//在独立线程中接收订阅通道中的消息
void Redis::observer_channel_message(){
	redisReply*reply=nullptr;
	while(REDIS_OK==redisGetReply(this->_subcribe_context,(void **)&reply))
	{
		//订阅收到的消息是一个带三元素的数组
		if(reply!=nullptr&&reply->element[2]!=nullptr&&reply->element[2]->str!=nullptr){
		//给业务层上报通道上发生的消息
		_notify_message_handler(atoi(reply->element[1]->str),reply->element[2]->str);
		}
		freeReplyObject(reply);
	}
	cerr<<">>>>>>>>>>>>>>>>>>>>>>>>>>>observer_channel_messgae quit<<<<<<<<<<<<<<<<"<<endl;
}

void Redis::init_notify_handler(function<void(int,string)> fn)
{
	this->_notify_message_handler=fn;
}

