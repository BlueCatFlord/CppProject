#ifndef REDIS_PROCESS_H
#define REDIS_PROCESS_H

#include <string>
#include <iostream>

#include "config/INIConfig.h"
#include "redis/hiredis.h"

#include "log4cplus/logger.h"
#include "log4cplus/loglevel.h"
#include "log4cplus/configurator.h"
#include "log4cplus/fileappender.h"
#include "log4cplus/loggingmacros.h"

using std::string;	

class RedisProcess
{
private:
	redisContext* _redisContext;
	redisReply* _redisReply;
	Config* config;
	static Logger logger;

public:
	RedisProcess(Config* _config);						//构造函数
	~RedisProcess();									//析构函数
	bool connect(string& host,unsigned int port);		//创建连接
	string get(string& key);							//根据key,获取value
	void set(string& key,string& value);				//存放key,value
};

#endif