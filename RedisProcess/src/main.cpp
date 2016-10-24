#include "RedisProcess.h"
#include "config/INIConfig.h"
#include "except/RedisException.h"

#include "log4cplus/logger.h"
#include "log4cplus/loglevel.h"
#include "log4cplus/configurator.h"
#include "log4cplus/fileappender.h"
#include "log4cplus/loggingmacros.h"

#include <string>
#include <iostream>
#include <cstdlib>

using std::string;
using std::cout;
using std::exception;
using namespace log4cplus;

string find(CONFIG_MAP* configMap, const string key);

int main(int argc, char *argv[])
{	
	log4cplus::initialize();
	PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("./conf/log4cplus.properties"));
	Logger root = Logger::getRoot();
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("RedisProcess"));		//获取到日志组件

	if (argc != 2)
	{
		LOG4CPLUS_ERROR(logger,"the args is error");
		return 0;
	}
	string configFile = argv[1];
	
	//构造配置对象
	Config *config = new Config();
	CONFIG_MAP* configMap;
	try
	{
		if (config->analyze(configFile))
		{
			configMap = config->getConfigMap();
		}
	}
	catch(RedisException& e)
	{
		LOG4CPLUS_ERROR(logger,e.what());
		return 0;		//直接返回，不做任何处理
	}

	//构造redis对象
	RedisProcess *redisProcess = new RedisProcess(config);
	const string hostField = "host";
	string host = find(configMap,hostField);
	const string portField = "port";
	string portStr = find(configMap,portField);
	unsigned int port = atoi(portStr.c_str());	
	try
	{
		if (!redisProcess->connect(host,port))
		{
			LOG4CPLUS_ERROR(logger, "connect the redis fail");
			return 0;
		}
	}
	catch(RedisException& e)
	{
		LOG4CPLUS_ERROR(logger, e.what());
		return 0;
	}

	string key = "BlueSky";
	string value = "433743";
	redisProcess->set(key,value);
	string result = redisProcess->get(key);
	LOG4CPLUS_DEBUG(logger, "the value for the key schaffer is "<< result);

	//release the redisProcess
	delete redisProcess;
	delete config;
	return 1;
}

//config map中查找key
string find(CONFIG_MAP* configMap,const string key)
{	
	string result=" ";
	if (!configMap->empty())
	{
		CONFIG_MAP::iterator iter = configMap->find(key);
		if (iter != configMap->end())
		{
			result = iter->second;
		}
	}
	return result;
}