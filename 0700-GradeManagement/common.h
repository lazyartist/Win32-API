#pragma once

#include "Windows.h"
#include <vector>

using namespace std;


// ===== define =====
#define Max_Account_Text 10 + 1
#define Max_Student_Id 6 + 1
#define Max_Student_Name 10 + 1
#define Max_Student_Info_Line (Max_Student_Id + 1 + Max_Student_Name)
#define Max_Student_File_Info 10 + 1

// ===== define ===== end


// ===== enum =====
enum LoginResultType
{
	Success, Fail
};
// ===== enum =====  end


// ===== struct =====
typedef struct _Student {
	char Id[Max_Student_Id];
	char Name[Max_Student_Name];

} Student, *PStudent;
// ===== struct ===== end


// ===== function =====
inline void log(LPCSTR lpStr) {
	OutputDebugString(lpStr);
	OutputDebugString("\n");
};
// ===== function ===== end
