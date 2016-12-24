#pragma once
#include "cEventBox.h"
#include <vector>

class cEventManager
{
	vector <cEventBox*> m_vEventBox;

public:
	cEventManager();
	~cEventManager();

	void release();
	void update();
	void PushEventBox(cEventBox* Eventbox);
	void Render();
};

