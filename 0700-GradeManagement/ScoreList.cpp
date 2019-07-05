#include "ScoreList.h"

ScoreList::ScoreList()
{
}

ScoreList::~ScoreList()
{
}

void ScoreList::LoadScore(const char * id, unsigned int semester)
{
	_vScores.clear();

	// 프로그램 내에서 파일을 여러번 열고 닫을 경우 fopen_s는 두번째부터 에러(13)가 나기 때문에 _fsopen()을 사용한다.
	//fopen_s(&file, "students.txt", "rt");
	char filePath[MAX_PATH];
	sprintf_s(filePath, Str_ScoreFile_Path_Format, id, semester);

	FILE *file;
	file = _fsopen(filePath, "rt", _SH_DENYNO/*Permits read and write access.*/);

	IsExistScore = file != nullptr;
	if (IsExistScore) {
		char strLineCount[Max_Score_Info_Line] = {};
		fgets(strLineCount, Max_Score_Info_Line, file);
		int nLineCount = atoi(strLineCount);

		char strScoreInfoLine[Max_Score_Info_Line] = {};
		char id[Max_Student_Id];
		char semester[Max_Score_Text];
		char course[Max_Score_Text];
		char point[Max_Score_Text];

		char *token;
		char *nextToken;

		_vScores.reserve(nLineCount); // 메모리 미리 할당

		for (size_t i = 0; i < nLineCount; i++)
		{
			memset(strScoreInfoLine, 0, Max_Score_Info_Line);
			fgets(strScoreInfoLine, Max_Score_Info_Line, file);

			token = strtok_s(strScoreInfoLine, "\t", &nextToken);
			strcpy_s(course, token);
			token = strtok_s(NULL, "\t", &nextToken);
			strcpy_s(point, token);

			// \n을 제거
			course[strcspn(course, "\n")] = 0;
			point[strcspn(point, "\n")] = 0;

			log(course);

			Score score(course, point);
			AddScore(&score);
		}
	}
}

void ScoreList::SaveScores(const char * id, unsigned int semesterIndex)
{
	// 파일 열기
	char filePath[MAX_PATH];
	sprintf_s(filePath, Str_ScoreFile_Path_Format, id, semesterIndex);

	FILE *file;
	file = _fsopen(filePath, "wt", _SH_DENYNO); // 경로에 폴더가 있다면 이미 존해해야함. 파일과 같이 생성하지 않음.

	if (file != nullptr) {
		char buffer[Max_Score_Info_Line];

		// 성적 수 기록
		int itemCount = _vScores.size();
		sprintf_s(buffer, Max_Score_Info_Line, "%d\n", itemCount);
		fputs(buffer, file);

		// 성적 정보 기록
		auto it = _vScores.begin();
		while (it != _vScores.end())
		{
			sprintf_s(buffer, Max_Score_Info_Line, "%s\t%s\n", it->Course, it->Point);
			fputs(buffer, file);

			++it;
		}
	}

	if (file) {
		fclose(file);
		log("SaveScores success!");
	}
}

void ScoreList::AddScore(Score *score)
{
	_vScores.push_back(*score);
}

void ScoreList::RemoveScore(int index)
{
	if (index != -1) {
		auto itBegin = _vScores.begin();
		auto itTarget = itBegin + index;
		_vScores.erase(itTarget);
	}
}

vector<Score>* ScoreList::GetScores()
{
	return &_vScores;
}
