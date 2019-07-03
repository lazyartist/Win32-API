#pragma once 

#include "File.h"

File::File(const char *fileName, const char *mode)
{
	fopen_s(&_pFile, "account.txt", mode);
}

File::~File()
{
	close();
}

void File::getLine(char *buffer, size_t bufferSize) {
	if (_pFile != nullptr) {
		fgets(buffer, bufferSize, _pFile);

		// \n은 줄바꿈을 지정하는 문자이므로 순수 문자만 얻기 위해 제거한다.
		buffer[strcspn(buffer, "\n")] = 0; // strcspn()으로 "\n"의 위치를 찾고 그 위치에 0을 넣어준다.
	}
}

void File::writeLine(const char *buffer, size_t bufferSize) {
	if (_pFile != nullptr) {
		fputs(buffer, _pFile);
	}
}

void File::close() {
	if (_pFile != nullptr) {
		fclose(_pFile);
	}
}