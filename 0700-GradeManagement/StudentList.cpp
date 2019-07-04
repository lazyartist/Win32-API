#include "StudentList.h"


StudentList::StudentList()
{
}

StudentList::StudentList(const char * filePath)
{
	LoadStudents(filePath);
}


StudentList::~StudentList()
{
}

void StudentList::LoadStudents(const char * filePath)
{
	// ���α׷� ������ ������ ������ ���� ���� ��� fopen_s�� �ι�°���� ����(13)�� ���� ������ 
		// _fsopen()�� ����Ѵ�.
		//fopen_s(&file, "students.txt", "rt");
	FILE *file;
	file = _fsopen(filePath, "rt", _SH_DENYNO/*Permits read and write access.*/);

	char studentsFileInfo[Max_Student_File_Info] = {};
	fgets(studentsFileInfo, Max_Student_Info_Line, file);
	int studentsCount = atoi(studentsFileInfo);

	char studentInfoLine[Max_Student_Info_Line] = {};
	char id[Max_Student_Id];
	char name[Max_Student_Name];
	char *token;
	char *nextToken;

	g_vStudents.clear();
	g_vStudents.reserve(studentsCount); // �޸� �̸� �Ҵ�

	for (size_t i = 0; i < studentsCount; i++)
	{
		fgets(studentInfoLine, Max_Student_Info_Line, file);

		token = strtok_s(studentInfoLine, "\t", &nextToken);
		strcpy_s(id, token);
		token = strtok_s(NULL, "\t", &nextToken);
		strcpy_s(name, token);

		// \n�� ����
		id[strcspn(id, "\n")] = 0;
		name[strcspn(name, "\n")] = 0;

		Student student = {};
		strcpy_s(student.Id, id);
		strcpy_s(student.Name, name);

		g_vStudents.push_back(student);
	}
}

void StudentList::SaveStudents()
{
	// ���� ����
	FILE *file;
	file = _fsopen("students.txt", "wt", _SH_DENYNO/*Permits read and write access.*/);

	if (file != nullptr) {
		char buffer[Max_Student_Info_Line];

		// �л� �� ���
		int itemCount = g_vStudents.size();
		sprintf_s(buffer, Max_Student_Info_Line, "%d\n", itemCount);
		fputs(buffer, file);

		// �л� ���� ���
		auto it = g_vStudents.begin();
		while (it != g_vStudents.end())
		{
			sprintf_s(buffer, Max_Student_Info_Line, "%s\t%s\n", it->Id, it->Name);
			fputs(buffer, file);

			++it;
		}
	}

	if (file) {
		fclose(file);
		log("SaveStudents success!");
	}
}

void StudentList::ClearStudents()
{
	g_vStudents.clear();
}

void StudentList::AddStudent(char *id, char *name)
{
	Student student = {};
	strcpy_s(student.Id, id);
	strcpy_s(student.Name, name);

	g_vStudents.push_back(student);
}

void StudentList::RemoveStudent(int index)
{
	if (index != -1) {
		auto itBegin = g_vStudents.begin();
		auto itTarget = itBegin + index;
		g_vStudents.erase(itTarget);
	}
}

vector<Student>* StudentList::GetStudents()
{
	return &g_vStudents;
}
