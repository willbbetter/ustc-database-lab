#pragma once
#include <cstdio>
class File
{
public:
	File() { }
	void create();
	File(const char* fileName, int* dir);
	void close(int* dir);

public:
	FILE* file = NULL;
	int dirSize = 0;
	int pageSize = 0;
	int posDataStart = 0;
	int pageCount = 0;
};