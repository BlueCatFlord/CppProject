#include <iostream>
#include <string>
#include <map>
#include <fstream>

#include "INIConfig.h"
#include "../except/RedisException.h"

using namespace std;

const char COMMENT_CHAR='#';

Logger Config::logger = Logger::getInstance(LOG4CPLUS_TEXT("RedisProcess"));		//声明日志定义组件

Config::Config()
{
	LOG4CPLUS_INFO(logger, "create the config object");
	this->configMap = new CONFIG_MAP;
}

Config::~Config()
{
	this->configMap = NULL;
	LOG4CPLUS_INFO(logger, "release the resources");
}

//判断字符是否为空
bool isSpace(char c)
{
	if (' ' == c || '\t' == c) 
		return true;
	return false;
}

//是否为注释
bool isCommentChar(char c)
{
	if (COMMENT_CHAR == c)
		return true;
	return false;
}

//map结构中插入数据
void map_insert(CONFIG_MAP* configMap,string &key,string &value)
{	
	configMap->insert(make_pair(key,value));
}

//去除配置项前后的空格
void trim(string sentent)
{
	if (sentent.empty())
		return;
	int index, start_pos,end_pos;
	for (index = 0; index < sentent.size(); ++index)
	{
		if (!isSpace(sentent[index]))
			break;
	}

	if (index == sentent.size())
	{
		return;			//sentence 为空串
	}
	start_pos = index;			//前部分的空格
	cout <<"the stat_pos is "<< start_pos <<"\n";
	//TODO 该处为什么进行长度-1
	for (index = sentent.size()-1; index >= 0; --index)
	{	
		if (!isSpace(sentent[index]))
			break;
	}

	if (index == 0)
		return;
	end_pos = index;
	cout <<"the end_pos is "<< end_pos <<"\n";
	sentent = sentent.substr(start_pos,end_pos-start_pos+1);		//截取字符串
}

//解析读取文中的每一行
bool analyzeLine(const string& line, string& key, string& value)
{
	if (line.empty())
		return false;
	trim(line);				//去除前后空格
	int start_pos = 0, end_pos = line.size()-1, pos;

	//查找是否存在注释
	if ((pos = line.find(COMMENT_CHAR)) != -1)
	{
		if (0 == pos)
			return false;		//行首为注释符
		end_pos = pos - 1;
	}

	//查看是否存在=号
	if ((pos = line.find('=')) == -1 )
		return false;

	string proc_line = line.substr(start_pos,start_pos + 1 - end_pos);
	
	//按照'='进行切割
	//获取key
	key = proc_line.substr(0, pos);
	//获取value
	value = proc_line.substr(pos+1, end_pos+1-(start_pos+1));
	return true;
}

//解析配置文件
bool Config::analyze(const string& configFile)
{	
	this->configMap->clear();					//清空map内容
	ifstream infile(configFile.c_str());		//文件输入流
	if (!infile)
	{
		LOG4CPLUS_ERROR(logger, "open the file error");
		throw RedisException("open the file error");
	}
	
	//处理每一行
	string line, key, value;
	while(getline(infile,line))
	{
		LOG4CPLUS_DEBUG(logger, "the line is "<< line);
		if (analyzeLine(line,key,value))
		{
			//(*(this->configMap))[key] = value;
			map_insert(this->configMap,key,value);
		}
	}

	infile.close();							//关闭资源文件
	return true;
}

//获取config map结构
CONFIG_MAP* Config::getConfigMap() const
{
	return this->configMap;
}
