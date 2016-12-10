#pragma once

class cXMesh;
class cTransform;
class cSkinnedAnimation;

class cBaseObject
{
public:
	cTransform*			pTransform;
	cXMesh*				pMesh;
	cBoundBox			BoundBox;
	bool				IgnoreCreateShadow;			//������� ���鶧 ���õǴ�

protected:
	bool				bActive;	//Ȱ��ȭ ����
	cSkinnedAnimation*	pSkinned;	//��Ų�� Animtion

public:
	cBaseObject();
	~cBaseObject();

	void Update(float timeDelta);

	void Render() {
		if (this->bActive)
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


	//���õ� �޽��� ���� �ٿ��ڽ��� ���� �Ѵ�.
	void ComputeBoundBox();

protected:

	//override �ؼ� ���ÿ�....
	virtual void BaseObjectEnable() {}							//BaseObject �� Ȱ��ȭ �ɶ� ����
	virtual void BaseObjectDisable() {}							//BaseObject �� ��Ȱ��ȭ �ɶ� ����
	virtual void BaseObjectUpdate(float timeDelta) {}			//BaseObject �� Update �� ����....
	virtual void BaseObjectNoActiveUpdate(float timeDelte) {}	//BaseObject �� ��Ȱ��ȭ�� ������Ʈ ����....

	virtual void BaseObjectRender();							//BaseObject �� �׸��� ����
};

