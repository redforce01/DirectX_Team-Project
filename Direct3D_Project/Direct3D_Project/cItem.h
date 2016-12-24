#pragma once
#include "cTransform.h"
class cXMesh_Static;
class cBoundBox;

class cAccessory : public cTransform
{
protected:
	cXMesh_Static*   mesh;
	cBoundBox      BoundBox;
	std::string      objName;

public:
	cAccessory() {}
	~cAccessory() {}

	HRESULT Init(
		std::string name, std::string filePath, D3DXVECTOR3 scale,
		D3DXVECTOR3 rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 translation = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	void Update(float timeDelta);
	void Release();
	void Render();
	void ComputeBoundBox();
	std::string GetObjectName() { return this->objName; }


};


class cGameItem : public cBaseObject
{
private:
	int m_type;
public:
	cGameItem()
	{
		this->bActive = true;
		this->pTransform = new cTransform;
	}
	~cGameItem() {}

	void Init(int type, std::string path, D3DXVECTOR3 trans, D3DXVECTOR3 scale, D3DXVECTOR3 rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	int GetItemType() { return this->m_type; }
};