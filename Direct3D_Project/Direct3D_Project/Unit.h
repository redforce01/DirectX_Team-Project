#pragma once
#include <map>
#include "cItem.h"
#include "cCloseEyeEvent.h"
#include "cObject_Sound.h"

#define MAX_STAMINA 100
#define HEAL_STAMINA 0.2f
#define MINUS_STAMINA -0.2f

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
class LeftArm;
class cEffect;
class cCameraUI;

class Unit
{
protected:
	int m_Hp;
	//현재 도는 Action의 IDX는 어디
	int m_ActionCurIdx;
	//움직이는가
	bool m_isDead;
	bool isMoving;
	bool isAttacking;
	bool m_isRayBlocking;
	bool m_isActive;
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

	cBoundBox* m_EventCollisionBox;
	//몸체의 
	cBoundBox* m_CollisionBox;
	//유닛의 충돌 구
	cBoundSphere* m_CollisionSphere;
	//유닛의 적을 감지하는 구?
	cBoundSphere* m_DetectSphere;
	
	vector<cObject_Sound*> m_vObjSound;

	cActionSeq* m_Action;
	Action* m_pCurAction;
	Unit* m_DetectedUnit;
	cScene* m_curScene;
	cEffect* m_blood;
	//	CollisionEvent* m_CollisionEvent;
	cCloseEyeEvent* m_Eye;
	cBoundSphere* ShortDetectSphere;
	//현재 상태 값을 나타내는 Class
	cCameraUI* m_CameraUI;
	cCamera* pMainCamera;
	Ray ray;
	// 현재 애니메이션 이름을 저장하기 위한 변수
	string m_Animation_Name;

	//스킨드 애니메이션 변수 
	cSkinnedAnimation*		pSkinnedAni;

	// 유닛의 현재 위치를 담기 위한 트랜스 폼
	cTransform*		pSkinnedTrans;

	// 유닛의 빛을 설정하기 위한 light 값
	std::vector<cLight*>	lights;

	float m_x, m_y, m_z;

	// 유닛의 XFile을 담은 변수
	string m_FilePath;
	cBaseObject* house;

	std::vector <cBaseObject*> m_vDoor;
	std::map< string, Action* > m_MState;
	std::map< string, Action* >::iterator m_MiState;
	std::vector <Unit*> m_vEnemy;
	std::vector <Unit*>::iterator m_viEnemy;
	// 현재 움직이는 상태니

	cTransform* m_ItemPocketTrans;
	cBoundBox* m_ItemPocketBound;

	bool m_bCamUp;
	bool isHoldingCam;
	bool m_isCanControll;
	bool m_isCanCamControll;
	bool m_isEventMod;

	bool m_isGet5RoomKey;
	bool m_isGetEscapeRoomKey;

	float m_CurStamina;
	float m_MaxStamina;
	int camUpDelayCount;
public:
	Unit() {};
	~Unit() {};

	virtual HRESULT Init() { return S_OK; };
	virtual void Update(float timeDelta) = 0;
	virtual void Release() = 0;
	// 처음 설정시 State Map에 State를 넣어주는 함수임. state 이름에 따라 애니메이션도 그 이름에 맞게 바뀜.
	// 예 ) 'RUN' State를 만들어서 넣으면, 'RUN' 애니메이션 넣은것과 같음.
	virtual void InitAnimation() = 0;
	//빛 초기화 해주는 함수,
	virtual void InitLight() = 0;
	virtual bool CollisionEvent(float timeDelta) = 0;

	virtual void Render();

	virtual void setDamage(int Damage) {};


	virtual void StatePlayChange(string StateName, float ChangeTime);  // State 바꾸고 그에 맞는 애니메이션 Play
	virtual void StateOneShotChange(string StateName, float ChangeTime);  // State 바꾸고 그에 맞는 애니메이션 OneShotPlay
	virtual void StateOneShotHoldChange(string StateName, float ChangeTime);  // State 바꾸고 그에 맞는 애니메이션 OneShotPlayandHold

	virtual void LerpMoveControll(float timeDelta, cNode * PreNode, cNode* NextNode);
	virtual Action* getAction() { return m_pCurAction; }
	virtual cSkinnedAnimation* getSkinned() { return pSkinnedAni; }
	virtual cTransform* getTrans() { return pSkinnedTrans; }
	virtual string getCurAniName() { return m_Animation_Name; }

	virtual cBoundSphere* getDetectSpere() { return m_DetectSphere; }
	virtual cBoundSphere* getCollisionSpere() { return m_CollisionSphere; }
	virtual cBoundBox* getCollisionBox() { return m_CollisionBox; }

	D3DXVECTOR3 getHeadPos() {
		//m_headPos.y += 0.15; 
		return m_headPos; }
	cTransform* getHeadTrans() { return pHeadTrans; }
	cTransform* getHeadCamTrans() { return m_pHeadCamTrans; }
	virtual int getHp() { return m_Hp;  }

	virtual void AttachToCamera(cTransform* camera);
	virtual void CameraAttachToUnit(cTransform* camera);

	virtual void pushEnemyUnitVector(Unit* unit);
	virtual bool SpDetectionCheck(Unit* Sub, Unit* Obj);
	virtual bool SpCollisionCheck(Unit* Sub, Unit* Obj);
	virtual bool BoxCollisionCheck(Unit* Sub, Unit* Obj);
	virtual void setPatroll(bool TF) {};
	virtual void setObj(cBaseObject* hos) { house = hos; }
	virtual void setRayCollision(bool TF) { m_isRayBlocking = TF; }
	virtual void TranslationHeadpos(float x, float y, float z) 
	{ 
		m_x = x;
		m_y = y;
		m_z = z;
	}

	virtual void setScene(cScene* scene) { m_curScene = scene; }

	virtual  void setMoving(bool TF) { isMoving = TF; }
	virtual bool getMoving() { return isMoving; }

	virtual  void setAttacking(bool TF) { isAttacking = TF; }
	virtual bool getAttacking() { return isAttacking; }

	virtual void setMove(int Idx) { m_ActionCurIdx = Idx; }
	virtual int getMove() { return m_ActionCurIdx; }

	virtual D3DXVECTOR3 getRightHandPos() { return m_rightHandPos; }
	virtual D3DXVECTOR3 getHeadCamPos()
	{  
		return m_headCamPos; 
	}
	virtual void setHeadCamPos(D3DXVECTOR3 vec)
	{
		m_headCamPos = vec;
	}

	virtual LPRay getRay() { return &ray; }
	virtual bool getRAy() { return m_isRayBlocking;  }
	virtual void setObjBox(vector <cBaseObject*> Obj) { };
	virtual bool getisActive() { return m_isActive; }
	virtual void setisActive(bool TF) { m_isActive = TF; }
	virtual void MemoryEffectGet(cEffect* effect) { m_blood = effect; }

	cTransform* GetItemPocketTrans() { return this->m_ItemPocketTrans; }
	cBoundBox* GetItemPocketBound() { return this->m_ItemPocketBound; }

	bool IsDead() { return this->m_isDead; }

	virtual void IsContoroll(bool TF) { m_isCanControll = TF; }
	virtual void IsCamContoroll(bool TF) { m_isCanCamControll = TF; }

	virtual void SetEventMod(bool TF) { m_isEventMod = TF; }
	virtual bool GetEventMod() { return m_isEventMod;  }
	void SetIsHoldingCam(bool is) { this->isHoldingCam = is; }
	bool GetIsHoldingCam() { return this->isHoldingCam; }
	virtual void EventCollisionEvent() {};
	virtual void SetMemoryCameraUI(class cCameraUI* CUI) { m_CameraUI = CUI;  }

	virtual void RunningStamina() {};
	virtual void PlusStamina() {};

	virtual float GetCurStamina() { return m_CurStamina; }
	virtual void StaminaSound() {};
	virtual void PlayDoorSound() {};
	virtual bool GetisFind() { return false; }

	bool IsCamUp() { return bCamUp; }
	void SetCamUpState(bool camUp) { bCamUp = camUp; }
	void SetDoorVec(vector<cBaseObject*>vDoor) { m_vDoor = vDoor; }
	//D3DXVECTOR3 getHeadPos() { return m_headPos; }

};


class PigEnemy : public Unit
{
	cTransform* m_LeftHandTrans;
	LeftArm* m_LArm;

	vector <vector <cNode*>> m_vWay;
	vector <cBaseObject*> m_vBox;
	map <pair<int, int>, Dijkstra*> m_mWay;
	map <pair<int, int>, vector<cTransform*>> m_mvWay;
	Dijkstra* m_Dijk;

	bool isPatroll;
	bool m_isFind;
	bool m_isAttacking;

	bool m_isChange;

	int walkTime;

public:
	PigEnemy(std::string filePath, D3DXVECTOR3 pos, cScene* Scene);

	~PigEnemy() {};

	virtual void Update(float timeDelta) override;
	virtual void Release() override;
	virtual void setPatroll(bool TF) { isPatroll = TF; }
	virtual bool CollisionEvent(float timeDelta);
	virtual bool RayCollisionCheck();
	virtual bool DoorRayCollisionCheck();

	virtual void NULLCollisionEvent();
	virtual void DetectCollisionEvent();
	virtual void BodyCollisionEvent();
	virtual void EventCollisionEvent();

	virtual void InitAnimation() override;
	virtual void InitLight() override;
	virtual void Render();

	virtual void PushActionSeq(vector<cTransform*> vWay, int MinusNum);
	virtual void AlongPlayerMove(cTransform* DestTrans);
	virtual void setObjBox(vector <cBaseObject*> Obj) { m_vBox = Obj; }

	virtual void PlayWalkSound();
	virtual void PlayRunSound();
	virtual void PlayDoorSound();
	virtual bool GetisFind() { return m_isFind; }

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

	//   HRESULT Init();
	virtual void Update(float timeDelta) override;
	virtual void Release() override;
	virtual bool CollisionEvent(float timeDelta) { return false; }
	virtual void EventCollisionEvent();
	virtual void    PlayerControll(float timeDelta);
	virtual void InitAnimation() override;
	virtual void InitLight() override;
	virtual void setDamage(int Damage);
	virtual void Render();


	//   void SetCamUpState(bool camUp){ this->m_bCamUp = camUp; }

	void CamUP_DOWNControll();
	void AttachToItem(cTransform* item);
	void AttachItemToUnit(cTransform* item);

	virtual void RunningStamina();
	virtual void PlusStamina();
	virtual void StaminaSound();


};

