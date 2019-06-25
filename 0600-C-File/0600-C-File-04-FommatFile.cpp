#include "pch.h"

void _0600_C_File_04_FormatFile() {
	cout << "=== format file output" << endl;
	{
		// open file stream
		FILE *file;
		fopen_s(&file, "formatFile.txt", "wt");

		// output text
		fprintf_s(file, "string: %s ,int : %d", "hihi", 99);

		// close file
		fclose(file);
	}

	cout << "=== format file input" << endl;
	{
		// open file stream
		FILE *file;
		fopen_s(&file, "formatFile.txt", "rt");

		// input text
		char str[99];
		int num;
		// �����̽� ���� ���ڱ��� %s�� ������ ������ �ְ� \n�� �����δ�.
		fscanf_s(file, "string: %s ,int : %d", str, _countof(str)/*string�� ũ��*/, &num);

		cout << str << endl;
		cout << num << endl;

		fclose(file);
	}

	typedef struct _A{
		int a;
		char b[10];
	} A;

	cout << "=== struct output" << endl;
	{
		A a;
		a.a = 99;
		strcpy_s(a.b, sizeof(a.b), "abcd");

		FILE *file;
		fopen_s(&file, "structFile.bin", "wb");

		fwrite(&a, sizeof(a), 1, file);

		fclose(file);
	}

	cout << "=== struct input" << endl;
	{
		A a = {};

		FILE *file;

		fopen_s(&file, "structFile.bin", "rb");
		fread_s(&a, sizeof(a), sizeof(a), 1, file);

		fclose(file);

		cout << a.a << endl;
		cout << a.b << endl;
	}

	cout << "=== fseek() output" << endl;
	{
		FILE *file = nullptr;
		fopen_s(&file, "fseek.txt", "wt");

		fputs("123456789", file);

		fclose(file);
	}

	cout << "=== fseek() input" << endl;
	{
		FILE *file = nullptr;
		fopen_s(&file, "fseek.txt", "rt");

		fseek(file, 2, SEEK_END);
		cout << fgetc(file) << endl; // -1 : ������ ���� �Ѿ �̵��߱� ������ -1�� ���

		fseek(file, -2, SEEK_END);
		char c = fgetc(file);
		cout << c << endl; // 8 : ������ ��(eof)���� 2ĭ �ڷ� �̵��ؼ� 8�� ����Ų��.
		// fgetc()�� 8�� ����� �Ŀ��� 9�� ����Ų��.
		
		cout << fgetc(file) << endl; // 57 : 9�� �ش��ϴ� �ƽ�Ű�ڵ�
		// fgetc()�� �ƽ�Ű�ڵ带 ��ȯ�ϱ� ������ �ٷ� ����ϸ� ���ڰ� ��µȴ�.
		// putchar(fgetc(file)); // putchar()�� �ƽ�Ű�ڵ带 ���� �޾� ǥ�� ��� ��Ʈ���� ����Ѵ�.

		fseek(file, 2, SEEK_SET);
		putchar(fgetc(file)); // 3 : ������ ó��(1)���� 2ĭ �̵��ؼ� 3�� �����Ѵ�.
		putchar('\n');
		// 3�� ����� �Ŀ��� 4�� ����Ų��.

		fseek(file, 1, SEEK_CUR);
		putchar(fgetc(file)); // 5 : ���� ��ġ 4���� �� ĭ�̵��ϴ� 5�� ����Ų��.
		putchar('\n');

		fclose(file);
	}

	cout << "=== ftell() output" << endl;
	{
		FILE *file = nullptr;
		fopen_s(&file, "ftell.txt", "wt");

		fputs("00100", file);

		fclose(file);
	}


	cout << "=== ftell() input" << endl;
	{
		FILE *file = nullptr;
		fopen_s(&file, "ftell.txt", "r+t"); // r+���� ����� ���� ������ ������ �� �ִ�.

		fseek(file, 2, SEEK_CUR); // ó������ 2ĭ �̵����� 1�� ����Ŵ
		int pos = ftell(file);
		cout << pos << endl; // 2 : 0���� ����

		fputc('9', file); // 1�� 9�����Ͽ� ����
		fclose(file);
		file = nullptr;

		// ���� �ٽ� ����
		fopen_s(&file, "ftell.txt", "rt");

		char str[10] = {};
		fgets(str, 10, file);
		fclose(file);

		cout << str << endl; // 00900
	}
}