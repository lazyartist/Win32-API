#include "pch.h"

void _0600_C_File_02_Text()
{
	// 파일 스트림
	printf("=== IO File");
	{
		FILE *file = {};

		// ===== 파일 쓰기 =====
		// '텍스트 읽기' 모드로 출력 스트림으로 읽기
		// 파일이 없으면 생성됨
		fopen_s(&file, "fopen_write.txt", "wt");

		// 파일 스트림에 문자 저장
		fputc('A', file);
		fputc('B', file);
		fputc('C', file);

		// 파일 스트림을 비움
		// 스트림에 있는 데이터를 파일에 기록(전송)
		fflush(file);

		// 파일 스트림 해제
		fclose(file);
		// ===== 파일 쓰기 ===== end


		// ===== 파일 읽기 ===== end
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
