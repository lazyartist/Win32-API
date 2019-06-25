#include "pch.h"

void _0600_C_File_01_IO()
{
	// 표준 스트림 문자 입출력
	printf("=== IO char ===");
	{
		int c1, c2;
		c1 = getchar();
		c2 = fgetc(stdin);
		// a를 입력하고 엔터를 누르면 c1에는 'a', c2에는 '\n'이 입력된다.
		// 엔터키도 아스키 코드 값이 10인 '\n'으로 표현되는 문자이기 때문
		/*
		a // a와 엔터를 입력
		*/

		//while ((c1 = getchar()) != EOF)
		//{
		//	putchar(c1);
		//}


		putchar(c1);
		fputc(c2, stdout);
		/*
		a1 // a와 (줄바꿈)이 출력
		*/
	}


	// 표준 스트림 문자열 입출력
	printf("=== IO string ===");
	{
		char str1[3];
		char str2[3];

		void *a = nullptr;
		a = gets_s(str1); // 성공 시 str1과 a의 포인터는 동일
		puts(str1);

		fgets(str2, sizeof(str2), stdin);
		fputs(str2, stdout);
	}
}
