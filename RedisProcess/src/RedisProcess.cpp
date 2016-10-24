#include "RedisProcess.h"
#include "except/RedisException.h"

#include <string>
#include <iostream>
#include <exception>

using std::string;
using std::cout;
using std::exception;

Logger RedisProcess::logger = Logger::getInstance(LOG4CPLUS_TEXT("RedisProcess"));	

RedisProcess::RedisProcess(Config* _config)
{
	LOG4CPLUS_DEBUG(logger, "create the object of redis-process");
	this->config = _config;
}

RedisProcess::~RedisProcess()
{
	this->_redisContext = NULL;
	this->_redisReply = NULL;
	this->config = NULL;
}

/**
* 创建与redis的连接
**/
bool RedisProcess::connect(string& host, unsigned int port)
{
	this->_redisContext = redisConnect(host.c_str(),port);
	if (this->_redisContext != NULL && this->_redisContext->err)
	{
		LOG4CPLUS_ERROR(logger, "connect redis error: " << this->_redisContext->errstr);
		throw RedisException("connect redis error");
		//return 0;		//连接失败
	}
	return 1;
}

/**
* 根据key,从redis中获取value
**/
string RedisProcess::get(string& key)
{
	this->_redisReply = (redisReply*)redisCommand(this->_redisContext,"GET %s",key.c_str());
	string reply = this->_redisReply->str;
	freeReplyObject(this->_redisReply);
	return reply;
}

/**
*  向redis中写入数据
**/
void RedisProcess::set(string& key,string& value)
{
	redisCommand(this->_redisContext,"SET %s %s",key.c_str(),value.c_str());
}