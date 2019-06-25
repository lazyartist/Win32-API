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
		// 스페이스 이전 문자까지 %s에 지정한 변수에 넣고 \n을 덧붙인다.
		fscanf_s(file, "string: %s ,int : %d", str, _countof(str)/*string의 크기*/, &num);

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
		cout << fgetc(file) << endl; // -1 : 파일의 끝을 넘어서 이동했기 때문에 -1이 출력

		fseek(file, -2, SEEK_END);
		char c = fgetc(file);
		cout << c << endl; // 8 : 파일의 끝(eof)에서 2칸 뒤로 이동해서 8을 가리킨다.
		// fgetc()로 8을 출력한 후에는 9를 가리킨다.
		
		cout << fgetc(file) << endl; // 57 : 9에 해당하는 아스키코드
		// fgetc()는 아스키코드를 반환하기 때문에 바로 출력하면 숫자가 출력된다.
		// putchar(fgetc(file)); // putchar()는 아스키코드를 값을 받아 표준 출력 스트림에 출력한다.

		fseek(file, 2, SEEK_SET);
		putchar(fgetc(file)); // 3 : 파일의 처음(1)에서 2칸 이동해서 3을 지정한다.
		putchar('\n');
		// 3을 출력한 후에는 4를 가리킨다.

		fseek(file, 1, SEEK_CUR);
		putchar(fgetc(file)); // 5 : 현재 위치 4에서 한 칸이동하니 5를 가리킨다.
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
		fopen_s(&file, "ftell.txt", "r+t"); // r+모드로 열어야 기존 내용을 유지할 수 있다.

		fseek(file, 2, SEEK_CUR); // 처음에서 2칸 이동으로 1을 가리킴
		int pos = ftell(file);
		cout << pos << endl; // 2 : 0부터 시작

		fputc('9', file); // 1을 9변경하여 저장
		fclose(file);
		file = nullptr;

		// 파일 다시 오픈
		fopen_s(&file, "ftell.txt", "rt");

		char str[10] = {};
		fgets(str, 10, file);
		fclose(file);

		cout << str << endl; // 00900
	}
}