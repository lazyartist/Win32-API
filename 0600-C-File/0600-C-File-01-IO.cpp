#include "pch.h"

void _0600_C_File_01_IO()
{
	// ǥ�� ��Ʈ�� ���� �����
	printf("=== IO char ===");
	{
		int c1, c2;
		c1 = getchar();
		c2 = fgetc(stdin);
		// a�� �Է��ϰ� ���͸� ������ c1���� 'a', c2���� '\n'�� �Էµȴ�.
		// ����Ű�� �ƽ�Ű �ڵ� ���� 10�� '\n'���� ǥ���Ǵ� �����̱� ����
		/*
		a // a�� ���͸� �Է�
		*/

		//while ((c1 = getchar()) != EOF)
		//{
		//	putchar(c1);
		//}


		putchar(c1);
		fputc(c2, stdout);
		/*
		a1 // a�� (�ٹٲ�)�� ���
		*/
	}


	// ǥ�� ��Ʈ�� ���ڿ� �����
	printf("=== IO string ===");
	{
		char str1[3];
		char str2[3];

		void *a = nullptr;
		a = gets_s(str1); // ���� �� str1�� a�� �����ʹ� ����
		puts(str1);

		fgets(str2, sizeof(str2), stdin);
		fputs(str2, stdout);
	}
}
