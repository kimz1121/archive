#pragma once
#ifndef PAD
#define PAD

#include <string>

using namespace std;

string pad(unsigned int pad)
{
	string str;
	for (unsigned int i = 0; i < pad; i++)
	{
		str = str + ' ';
	}
	return str;
}

string pad(char char_pad, unsigned int pad)
{
	string str;
	for (unsigned int i = 0; i < pad; i++)
	{
		str = str + char_pad;
	}
	return str;
}

#endif