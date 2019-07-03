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

		// \n�� �ٹٲ��� �����ϴ� �����̹Ƿ� ���� ���ڸ� ��� ���� �����Ѵ�.
		buffer[strcspn(buffer, "\n")] = 0; // strcspn()���� "\n"�� ��ġ�� ã�� �� ��ġ�� 0�� �־��ش�.
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