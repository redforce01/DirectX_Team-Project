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
	//���� ���� Action�� IDX�� ���
	int m_ActionCurIdx;
	//�����̴°�
	bool isMoving;
	bool isAttacking;
	//�밡�� ��ġ
	D3DXVECTOR3 m_headPos;
	//�밡�� Trans
	cTransform*	 pHeadTrans;
	cTransform*	 pHeadTrans2;

	//�밡�� ī�޶� Trans
	cTransform*	 m_pHeadCamTrans;

	//�밡�� ī�޶� ��ġ
	D3DXVECTOR3 m_headCamPos;

	//*�޼� ��ġ
	D3DXVECTOR3 m_rightHandPos;

	////*�޼� Trans
	cTransform* m_pRightHandTrans;


	//��ü�� 
	cBoundBox* m_CollisionBox;
	//������ �浹 ��
	cBoundSphere* m_CollisionSphere;
	//������ ���� �����ϴ� ��?
	cBoundSphere* m_DetectSphere;

	cActionSeq* m_Action;
	Action* m_pCurAction;
	Unit* m_DetectedUnit;
	cScene* m_curScene;

//	CollisionEvent* m_CollisionEvent;

	cBoundSphere* ShortDetectSphere;
	//���� ���� ���� ��Ÿ���� Class
	
	Ray ray;
	// ���� �ִϸ��̼� �̸��� �����ϱ� ���� ����
	string m_Animation_Name;

	//��Ų�� �ִϸ��̼� ���� 
	cSkinnedAnimation*		pSkinnedAni;

	// ������ ���� ��ġ�� ��� ���� Ʈ���� ��
	cTransform*		pSkinnedTrans;

	// ������ ���� �����ϱ� ���� light ��
	std::vector<cLight*>	lights;

	// ������ XFile�� ���� ����
	string m_FilePath;
	cBaseObject* house;
	std::map< string , Action* > m_MState;
	std::map< string, Action* >::iterator m_MiState;
	std::vector <Unit*> m_vEnemy;
	std::vector <Unit*>::iterator m_viEnemy;
	// ���� �����̴� ���´�

public:
	Unit() {};
	~Unit() {};

	virtual HRESULT Init() { return S_OK;  };
	virtual void Update(float timeDelta) = 0;
	virtual void Release() = 0;
	// ó�� ������ State Map�� State�� �־��ִ� �Լ���. state �̸��� ���� �ִϸ��̼ǵ� �� �̸��� �°� �ٲ�.
	// �� ) 'RUN' State�� ���� ������, 'RUN' �ִϸ��̼� �����Ͱ� ����.
	virtual void InitAnimation() = 0;
	//�� �ʱ�ȭ ���ִ� �Լ�,
	virtual void InitLight() = 0;
	virtual bool CollisionEvent(float timeDelta) = 0;

	virtual void Render();


	virtual void StatePlayChange(string StateName, float ChangeTime);  // State �ٲٰ� �׿� �´� �ִϸ��̼� Play
	virtual void StateOneShotChange(string StateName, float ChangeTime);  // State �ٲٰ� �׿� �´� �ִϸ��̼� OneShotPlay
	virtual void StateOneShotHoldChange(string StateName, float ChangeTime);  // State �ٲٰ� �׿� �´� �ִϸ��̼� OneShotPlayandHold

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

