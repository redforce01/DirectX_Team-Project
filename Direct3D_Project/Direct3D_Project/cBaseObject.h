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
	bool				IgnoreCreateShadow;			//쉐도우맵 만들때 무시되니
	std::string objName;

protected:
	bool				bActive;	//활성화 여부
	cSkinnedAnimation*	pSkinned;	//스킨드 Animtion

public:
	cBaseObject();
	~cBaseObject();

	void Update(float timeDelta);

	void Render() {
		//if (this->bActive)
			this->BaseObjectRender();
	}

	//랜더링 될 메쉬를 셋팅한다.
	void SetMesh(cXMesh*	pMesh);

	//활성화 셋팅
	void SetActive(bool bActive);

	//활성화 여부를 얻는다.
	bool IsActive() {
		return this->bActive;
	}
	void setObjectName(std::string name) { this->objName = name; }
	std::string GetObjectName() { return this->objName; }

	//셋팅된 메쉬에 따라 바운드박스를 재계산 한다.
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

	//override 해서 쓰시오....
	virtual void BaseObjectEnable() {}							//BaseObject 가 활성화 될때 실행
	virtual void BaseObjectDisable() {}							//BaseObject 가 비활성화 될때 실행
	virtual void BaseObjectUpdate(float timeDelta) {}			//BaseObject 가 Update 때 실행....
	virtual void BaseObjectNoActiveUpdate(float timeDelte) {}	//BaseObject 가 비활성화시 업데이트 실행....

	virtual void BaseObjectRender();							//BaseObject 를 그릴때 실행
};

