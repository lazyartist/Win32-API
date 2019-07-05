#pragma once

#include <iostream>
#include <vector>
#include "common.h"

class StudentList
{
public:
	StudentList();
	StudentList(const char * filePath);
	~StudentList();

	void LoadStudents(const char * filePath);
	void SaveStudents();
	void ClearStudents();
	void AddStudent(char *id, char *name);

	void RemoveStudent(int index);

	vector<Student>* GetStudents();

private:
	vector<Student> _vStudents;
};

