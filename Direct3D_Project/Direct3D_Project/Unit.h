#pragma once
#include <map>
#include "cItem.h"

class cScene;
class cActionSeq;
class cTransform;
class cXMesh_Skinned;
class cSkinnedAnimation;
class cLight;
class Action;
class Dijkstra;
class cCamera;
class cNode;
class cBoundBox;
class cBoundSphere;
class cBaseObject;
class InputHandler;
class Dijkstra;
class MoveMap;

class Unit
{
protected:
	//현재 도는 Action의 IDX는 어디
	int m_ActionCurIdx;
	//움직이는가
	bool isMoving;
	bool isAttacking;
	//대가리 위치
	D3DXVECTOR3 m_headPos;
	//대가리 Trans
	cTransform*	 pHeadTrans;
	cTransform*	 pHeadTrans2;

	//대가리 카메라 Trans
	cTransform*	 m_pHeadCamTrans;

	//대가리 카메라 위치
	D3DXVECTOR3 m_headCamPos;

	//*왼손 위치
	D3DXVECTOR3 m_rightHandPos;

	////*왼손 Trans
	cTransform* m_pRightHandTrans;


	//몸체의 
	cBoundBox* m_CollisionBox;
	//유닛의 충돌 구
	cBoundSphere* m_CollisionSphere;
	//유닛의 적을 감지하는 구?
	cBoundSphere* m_DetectSphere;

	cActionSeq* m_Action;
	Action* m_pCurAction;
	Unit* m_DetectedUnit;
	cScene* m_curScene;

//	CollisionEvent* m_CollisionEvent;

	cBoundSphere* ShortDetectSphere;
	//현재 상태 값을 나타내는 Class
	
	Ray ray;
	// 현재 애니메이션 이름을 저장하기 위한 변수
	string m_Animation_Name;

	//스킨드 애니메이션 변수 
	cSkinnedAnimation*		pSkinnedAni;

	// 유닛의 현재 위치를 담기 위한 트랜스 폼
	cTransform*		pSkinnedTrans;

	// 유닛의 빛을 설정하기 위한 light 값
	std::vector<cLight*>	lights;

	// 유닛의 XFile을 담은 변수
	string m_FilePath;
	cBaseObject* house;
	std::map< string , Action* > m_MState;
	std::map< string, Action* >::iterator m_MiState;
	std::vector <Unit*> m_vEnemy;
	std::vector <Unit*>::iterator m_viEnemy;
	// 현재 움직이는 상태니

public:
	Unit() {};
	~Unit() {};

	virtual HRESULT Init() { return S_OK;  };
	virtual void Update(float timeDelta) = 0;
	virtual void Release() = 0;
	// 처음 설정시 State Map에 State를 넣어주는 함수임. state 이름에 따라 애니메이션도 그 이름에 맞게 바뀜.
	// 예 ) 'RUN' State를 만들어서 넣으면, 'RUN' 애니메이션 넣은것과 같음.
	virtual void InitAnimation() = 0;
	//빛 초기화 해주는 함수,
	virtual void InitLight() = 0;
	virtual bool CollisionEvent(float timeDelta) = 0;

	virtual void Render();


	virtual void StatePlayChange(string StateName, float ChangeTime);  // State 바꾸고 그에 맞는 애니메이션 Play
	virtual void StateOneShotChange(string StateName, float ChangeTime);  // State 바꾸고 그에 맞는 애니메이션 OneShotPlay
	virtual void StateOneShotHoldChange(string StateName, float ChangeTime);  // State 바꾸고 그에 맞는 애니메이션 OneShotPlayandHold

	virtual void LerpMoveControll(float timeDelta, cNode * PreNode, cNode* NextNode);
	virtual Action* getAction() { return m_pCurAction; }
	virtual cSkinnedAnimation* getSkinned() { return pSkinnedAni;  }
	virtual cTransform* getTrans() { return pSkinnedTrans; }
	virtual string getCurAniName() { return m_Animation_Name; }

	virtual cBoundSphere* getDetectSpere() { return m_DetectSphere;  }
	virtual cBoundSphere* getCollisionSpere() { return m_CollisionSphere; }
	virtual cBoundSphere* getCollisionBox() { return m_CollisionBox; }

    D3DXVECTOR3 getHeadPos() { m_headPos.y += 0.15; return m_headPos; }
	cTransform* getHeadTrans() { return pHeadTrans;  }


	virtual void AttachToCamera(cTransform* camera);
	virtual void CameraAttachToUnit(cTransform* camera);

	virtual void pushEnemyUnitVector(Unit* unit);
	virtual bool SpDetectionCheck(Unit* Sub, Unit* Obj);
	virtual bool SpCollisionCheck(Unit* Sub, Unit* Obj);
	virtual bool BoxCollisionCheck(Unit* Sub, Unit* Obj);
	virtual void setPatroll(bool TF) {};
	virtual void setObj(cBaseObject* hos) { house = hos;  }

	virtual void setScene(cScene* scene) { m_curScene = scene; }

	virtual  void setMoving(bool TF) { isMoving = TF; }
	virtual bool getMoving() { return isMoving;  }

	virtual  void setAttacking(bool TF) { isAttacking = TF; }
	virtual bool getAttacking() { return isAttacking; }

	virtual void setMove(int Idx) { m_ActionCurIdx = Idx; }
	virtual int getMove() { return m_ActionCurIdx;  }

	D3DXVECTOR3 getRightHandPos(){ return m_rightHandPos;}

	D3DXVECTOR3 getHeadCamPos() {return m_headCamPos;}

	//D3DXVECTOR3 getHeadPos() { return m_headPos; }

};


class PigEnemy : public Unit
{

	vector <vector <cNode*>> m_vWay;
	map <pair<int,int>, Dijkstra*> m_mWay;
	Dijkstra* m_Dijk;
	bool isPatroll;
	bool m_isFind;
	bool m_isAttacking;

	bool m_isChange;

public :
	PigEnemy(std::string filePath, D3DXVECTOR3 pos, cScene* Scene);

	~PigEnemy() {};

	virtual void Update(float timeDelta) override;
	virtual void Release() override;
	virtual void setPatroll(bool TF) { isPatroll = TF; }
	virtual bool CollisionEvent(float timeDelta);

	virtual void InitAnimation() override;
	virtual void InitLight() override;

	virtual void PushActionSeq(vector<cTransform*> vWay, int MinusNum);
	virtual void AlongPlayerMove(cTransform* DestTrans);

};

class Player : public Unit
{
private:
	InputHandler* input;

	D3DXVECTOR3 handydcamPos;

	cAccessory* head;
	cAccessory* handycam;
	vector<cAccessory*> vecItem;

public:
	Player(std::string filePath, D3DXVECTOR3 pos);
	~Player() {};

//	HRESULT Init();
	virtual void Update(float timeDelta) override;
	virtual void Release() override;
	virtual bool CollisionEvent(float timeDelta) { return false; }

	virtual void 	PlayerControll(float timeDelta);
	virtual void InitAnimation() override;
	virtual void InitLight() override;
	virtual void SitDown();

	virtual void Render();

	void AttachToItem(cTransform* item);
	void AttachItemToUnit(cTransform* item);


};

