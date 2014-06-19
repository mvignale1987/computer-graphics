#pragma once

#include <string>

class CPUInfo
{
public:
	std::string vendor;
	int cores;
	int logical;
	bool hyperThreads;
public:
	static CPUInfo get();
};

