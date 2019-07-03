#pragma once

#include <stdio.h>
#include "common.h"

class File
{
public:
	File(const char * fileName, const char * mode);
	~File();

	void getLine(char * buffer, size_t bufferSize);

	void writeLine(const char * buffer, size_t bufferSize);

	void close();

private:
	FILE *_pFile;
};

