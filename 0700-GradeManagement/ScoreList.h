#pragma once

#include <vector>
#include "common.h"

class ScoreList
{
public:
	ScoreList();
	//ScoreList(const char * id, unsigned int semester);
	~ScoreList();

	void LoadScore(const char * id, unsigned int semester);
	void SaveScores(const char * id, unsigned int semesterIndex);
	void ClearScore();
	void AddScore(Score *score);

	void RemoveScore(int index);

	vector<Score>* GetScores();

	bool IsExistScore = false;

private:
	vector<Score> _vScores;
};

