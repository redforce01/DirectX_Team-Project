#pragma once

class cXMesh;
class cTransform;
class cSkinnedAnimation;

class cBaseObject
{
public:
	Ray m_ray;
	cTransform*			pTransform;
	cXMesh*				pMesh;
	cBoundBox			BoundBox;
	bool				IgnoreCreateShadow;			//������� ���鶧 ���õǴ�
	std::string objName;

protected:
	bool				bActive;	//Ȱ��ȭ ����
	cSkinnedAnimation*	pSkinned;	//��Ų�� Animtion

public:
	cBaseObject();
	~cBaseObject();

	void Update(float timeDelta);

	void Render() {
		//if (this->bActive)
			this->BaseObjectRender();
	}

	//������ �� �޽��� �����Ѵ�.
	void SetMesh(cXMesh*	pMesh);

	//Ȱ��ȭ ����
	void SetActive(bool bActive);

	//Ȱ��ȭ ���θ� ��´�.
	bool IsActive() {
		return this->bActive;
	}
	void setObjectName(std::string name) { this->objName = name; }
	std::string GetObjectName() { return this->objName; }

	//���õ� �޽��� ���� �ٿ��ڽ��� ���� �Ѵ�.
	void ComputeBoundBox();
	void ComputeBoundBox(float radius);
	virtual void settingPoint() { };
	virtual void ComputeRay(Ray ray) {}; 
	virtual void setLock(bool TF) { };

	virtual void InnerOpen() {};
	virtual void OutterOpen() {};

	virtual void CloseDoor() {};
	virtual bool getOpen() { return false; }

protected:
	virtual void setOpen(bool TF) {  }

	//override �ؼ� ���ÿ�....
	virtual void BaseObjectEnable() {}							//BaseObject �� Ȱ��ȭ �ɶ� ����
	virtual void BaseObjectDisable() {}							//BaseObject �� ��Ȱ��ȭ �ɶ� ����
	virtual void BaseObjectUpdate(float timeDelta) {}			//BaseObject �� Update �� ����....
	virtual void BaseObjectNoActiveUpdate(float timeDelte) {}	//BaseObject �� ��Ȱ��ȭ�� ������Ʈ ����....

	virtual void BaseObjectRender();							//BaseObject �� �׸��� ����
};

