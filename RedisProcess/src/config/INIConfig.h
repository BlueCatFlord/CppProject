#ifndef INI_CONFIG_H
#define INI_CONFIG_H

#include "log4cplus/logger.h"
#include "log4cplus/loglevel.h"
#include "log4cplus/configurator.h"
#include "log4cplus/fileappender.h"
#include "log4cplus/loggingmacros.h"

#include <string>
#include <map>


using namespace std;
using namespace log4cplus;

typedef map<string,string> CONFIG_MAP; 	

class Config
{
private:
	CONFIG_MAP* configMap;
	static Logger logger;
public:
	CONFIG_MAP* getConfigMap() const;					//获取参数map
	bool analyze(const string& configFile);				//解析配置文件
	Config();
	~Config();
};

#endif
