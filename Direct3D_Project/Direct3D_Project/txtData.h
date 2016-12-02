#pragma once

#include "cSingletonBase.h"
#include <vector>
#include <string>

class txtData : public cSingletonBase<txtData>
{
public:
	txtData();
	~txtData();

	HRESULT init();
	void release();

	//���̺�
	void txtSave(const char* saveFileName, vector<string> vStr);
	char* vectorArrayCombine(vector<string> vArray);

	//�ε�
	vector<string> txtLoad(const char* loadFileName);
	vector<string> charArraySeparation(char charArray[]);
};

#define TXTDATA txtData::GetInstance()