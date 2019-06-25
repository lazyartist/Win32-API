#include "pch.h"

#define NumCount 10

void _0600_C_File_03_Binary() {
	cout << "=== binary file output" << endl;
	{
		FILE *pFile; // 파일 구조체 포인터 선언
		fopen_s(&pFile, "fopen_binary.bin", "wb"); // 파일 출력 스트림 얻기

		for (size_t i = 0; i < NumCount; i++)
		{
			int buf = i;
			fwrite(&buf, sizeof(size_t), 1, pFile); // 파일에 size_t 데이터를 바이너리로 기록
		}

		fclose(pFile); // 파일 스트림 닫기
	}

	cout << "=== binary file input" << endl;
	{
		FILE *pFile; // 파일 구조체 포인터 선언
		fopen_s(&pFile, "fopen_binary.bin", "rb"); // 파일 입력 스트림 얻기

		int buf[NumCount] = {}; // 읽은 데이터를 저장할 버퍼
		fread(buf, sizeof(size_t), NumCount, pFile); // 파일의 바이너리를 size_t 크기로 읽기

		for (size_t i = 0; i < NumCount; i++)
		{
			cout << buf[i]; // 읽은 데이터를 하나씩 출력
		}
		cout << endl;

		fclose(pFile);
	}

	cout << "=== mp3 file copy" << endl;
	{
		FILE *pReadFile;
		FILE *pWriteFile;
		fopen_s(&pReadFile, "SampleAudio.mp3", "rb");
		fopen_s(&pWriteFile, "SampleAudio_Copy.mp3", "wb");
		
		size_t bufferSize = 1;
		size_t elementSize = sizeof(unsigned char);
		size_t elementCount = 1;
		for (size_t i = 0; i < 500000; i++) // 434KB(443,926 bytes) 파일을 복사
		{
			unsigned char buf;
			size_t result = fread_s(&buf, bufferSize, elementSize, elementCount, pReadFile);

			// 입력 파일 스트림의 끝인지 검사
			// result가 elementCount보다 작아도 에러 때문일 수 있으므로 feof로 끝인지 검사한다.
			if (result < 1 && feof(pReadFile))
			{
				break;
			}

			fwrite(&buf, elementSize, elementCount, pWriteFile); // 입력 스트림에서 가져온 데이터를 출력 스트림에 쓴다.
		}

		// 파일 스트림 정리
		fclose(pReadFile);
		fclose(pWriteFile);
	}
}