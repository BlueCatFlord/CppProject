#ifndef REDIS_EXCEPTION_H
#define REDIS_EXCEPTION_H

#include <iostream>
#include <string>
#include <exception>

using namespace std;

class RedisException
{
private:
	const char* msg;
public:
	RedisException(const char* _msg);
	~RedisException();
	const char * what();
};

#endif