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

	char studentsFileInfo[Max_Student_Info_Line] = {};
	fgets(studentsFileInfo, Max_Student_Info_Line, file);
	int studentsCount = atoi(studentsFileInfo);

	char studentInfoLine[Max_Student_Info_Line] = {};
	char id[Max_Student_Id];
	char name[Max_Student_Name];
	char *token;
	char *nextToken;

	_vStudents.clear();
	_vStudents.reserve(studentsCount); // �޸� �̸� �Ҵ�

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

		AddStudent(id, name);
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
		int itemCount = _vStudents.size();
		sprintf_s(buffer, Max_Student_Info_Line, "%d\n", itemCount);
		fputs(buffer, file);

		// �л� ���� ���
		auto it = _vStudents.begin();
		while (it != _vStudents.end())
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
	_vStudents.clear();
}

void StudentList::AddStudent(char *id, char *name)
{
	Student student = {};
	strcpy_s(student.Id, id);
	strcpy_s(student.Name, name);

	_vStudents.push_back(student);
}

void StudentList::RemoveStudent(int index)
{
	if (index != -1) {
		auto itBegin = _vStudents.begin();
		auto itTarget = itBegin + index;
		_vStudents.erase(itTarget);
	}
}

vector<Student>* StudentList::GetStudents()
{
	return &_vStudents;
}
