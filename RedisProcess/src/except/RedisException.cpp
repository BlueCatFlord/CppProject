#include "RedisException.h"

#include <iostream>

using namespace std;

RedisException::RedisException(const char* _msg)
:msg(_msg)
{}

//release resources
RedisException::~RedisException()
{
	delete this->msg;
}

const char* RedisException::what()
{
	return this->msg;
}