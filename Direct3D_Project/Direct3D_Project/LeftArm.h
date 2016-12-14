#pragma once

class cTransform;
class cBoundBox;

class LeftArm
{

	bool m_isActive;
	cTransform* m_Trans;
	cBoundBox* m_Box;



public:
	LeftArm();
	~LeftArm();

	void AddToBody(cTransform* cBody);


	void setActive(bool TF) { m_isActive = TF; }
	bool getisActive() { return m_isActive;  }

	cTransform* getTrans() { return m_Trans; }
	cBoundBox* getBox() { return m_Box; }



};

