#include "pch.h"

#define NumCount 10

void _0600_C_File_03_Binary() {
	cout << "=== binary file output" << endl;
	{
		FILE *pFile; // ���� ����ü ������ ����
		fopen_s(&pFile, "fopen_binary.bin", "wb"); // ���� ��� ��Ʈ�� ���

		for (size_t i = 0; i < NumCount; i++)
		{
			int buf = i;
			fwrite(&buf, sizeof(size_t), 1, pFile); // ���Ͽ� size_t �����͸� ���̳ʸ��� ���
		}

		fclose(pFile); // ���� ��Ʈ�� �ݱ�
	}

	cout << "=== binary file input" << endl;
	{
		FILE *pFile; // ���� ����ü ������ ����
		fopen_s(&pFile, "fopen_binary.bin", "rb"); // ���� �Է� ��Ʈ�� ���

		int buf[NumCount] = {}; // ���� �����͸� ������ ����
		fread(buf, sizeof(size_t), NumCount, pFile); // ������ ���̳ʸ��� size_t ũ��� �б�

		for (size_t i = 0; i < NumCount; i++)
		{
			cout << buf[i]; // ���� �����͸� �ϳ��� ���
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
		for (size_t i = 0; i < 500000; i++) // 434KB(443,926 bytes) ������ ����
		{
			unsigned char buf;
			size_t result = fread_s(&buf, bufferSize, elementSize, elementCount, pReadFile);

			// �Է� ���� ��Ʈ���� ������ �˻�
			// result�� elementCount���� �۾Ƶ� ���� ������ �� �����Ƿ� feof�� ������ �˻��Ѵ�.
			if (result < 1 && feof(pReadFile))
			{
				break;
			}

			fwrite(&buf, elementSize, elementCount, pWriteFile); // �Է� ��Ʈ������ ������ �����͸� ��� ��Ʈ���� ����.
		}

		// ���� ��Ʈ�� ����
		fclose(pReadFile);
		fclose(pWriteFile);
	}
}