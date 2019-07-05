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

	// ���α׷� ������ ������ ������ ���� ���� ��� fopen_s�� �ι�°���� ����(13)�� ���� ������ _fsopen()�� ����Ѵ�.
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

		_vScores.reserve(nLineCount); // �޸� �̸� �Ҵ�

		for (size_t i = 0; i < nLineCount; i++)
		{
			memset(strScoreInfoLine, 0, Max_Score_Info_Line);
			fgets(strScoreInfoLine, Max_Score_Info_Line, file);

			token = strtok_s(strScoreInfoLine, "\t", &nextToken);
			strcpy_s(course, token);
			token = strtok_s(NULL, "\t", &nextToken);
			strcpy_s(point, token);

			// \n�� ����
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
	// ���� ����
	char filePath[MAX_PATH];
	sprintf_s(filePath, Str_ScoreFile_Path_Format, id, semesterIndex);

	FILE *file;
	file = _fsopen(filePath, "wt", _SH_DENYNO); // ��ο� ������ �ִٸ� �̹� �����ؾ���. ���ϰ� ���� �������� ����.

	if (file != nullptr) {
		char buffer[Max_Score_Info_Line];

		// ���� �� ���
		int itemCount = _vScores.size();
		sprintf_s(buffer, Max_Score_Info_Line, "%d\n", itemCount);
		fputs(buffer, file);

		// ���� ���� ���
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
