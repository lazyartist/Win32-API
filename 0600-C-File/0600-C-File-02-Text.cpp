#include "pch.h"

void _0600_C_File_02_Text()
{
	// ���� ��Ʈ��
	printf("=== IO File");
	{
		FILE *file = {};

		// ===== ���� ���� =====
		// '�ؽ�Ʈ �б�' ���� ��� ��Ʈ������ �б�
		// ������ ������ ������
		fopen_s(&file, "fopen_write.txt", "wt");

		// ���� ��Ʈ���� ���� ����
		fputc('A', file);
		fputc('B', file);
		fputc('C', file);

		// ���� ��Ʈ���� ���
		// ��Ʈ���� �ִ� �����͸� ���Ͽ� ���(����)
		fflush(file);

		// ���� ��Ʈ�� ����
		fclose(file);
		// ===== ���� ���� ===== end


		// ===== ���� �б� ===== end
		fopen_s(&file, "fopen_write.txt", "rt");
		for (size_t i = 0; i < 100; i++)
		{
			char ch = fgetc(file);

			if (EOF == ch) {
				break;
			}
			printf("%c\n", ch);
		}

		if (feof(file) != 0) {
			printf("end of file");
		}
	}

	char str2[3];
	fgets(str2, sizeof(str2), stdin);
	fputs(str2, stdout);
}
