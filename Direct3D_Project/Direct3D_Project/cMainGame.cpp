#include "StdAfx.h"
#include "cMainGame.h"
#include "cImage.h"
#include "cTitle.h"
#include "cScene_Game.h"
#include "cEndingMain.h"
#include "cScene_Loading.h"

float g_Czoom = 0.0f;
int zDelta = 0.0f;
bool bCamUp = false;

cMainGame::cMainGame( void )
{
}

cMainGame::~cMainGame( void )
{
}

//����
HRESULT cMainGame::Init( void )
{
	//����̽� ����
	CreateDevice();

	//�Ŵ��� ����
	TIME_MGR->Init();
	LOG_MGR->Init( LOG_WINDOW | LOG_FILE, g_hWnd, "Dump" );
	DXFONT_MGR->Init(Device);
	GIZMO_MGR->Init(Device);
	SPRITE_MGR->Init(Device);
	SCENE_MGR->Init();
	EVENT_MGR->Init();

	SOUNDMANAGER->init();
	SOUNDDATA->Init();

	SCENE_MGR->AddScene("title", new cTitle);
	SCENE_MGR->AddScene("game", new cScene_Game);
	SCENE_MGR->AddScene("ending", new cEndingMain);

	SCENE_MGR->AddLoadingScene("loading", new cScene_Loading);
	//SCENE_MGR->AddScene("loading", new cScene_Loading);
	
	SCENE_MGR->ChangeScene("title");

	//DXFONT_MGR->addStyle(Device, "������", "���긲", 30.f);

	//������
	//m_pNowScene = new cScene_Main();

	//���ʱ�ȭ
	//if (FAILED(m_pNowScene->Init()))		//���ϰ��� E_FAIL;
	//	return E_FAIL;


	//return E_FAIL;
	return S_OK;
}

//����
void cMainGame::Release()
{

	//������
	//m_pNowScene->Release();
	//SAFE_DELETE( m_pNowScene );

	//�Ŵ��� ����
	TIME_MGR->Release();
	cTimeMgr::ReleaseInstance();
	KEY_MGR->Release();
	cKeyMgr::ReleaseInstance();
	LOG_MGR->Release();
	cLogMgr::ReleaseInstance();
	GIZMO_MGR->Release();
	cGizmoManager::ReleaseInstance();
	SCENE_MGR->Release();
	cScene_Mgr::ReleaseInstance();

	cPhysicManager::ReleaseInstance();

	EVENT_MGR->Release();
	cEventObjectManager::ReleaseInstance();
	RESOURCE_TEXTURE->ClearResource();
	cResourceMgr_Texture::ReleaseInstance();
	RESOURCE_TEXTURE->ClearResource();
	cResourceMgr_Texture::ReleaseInstance();
	RESOURCE_FX->ClearResource();
	cResourceMgr_ShaderFX::ReleaseInstance();
	RESOURCE_STATICXMESH->ClearResource();
	cResourceMgr_XStaticMesh::ReleaseInstance();

	SOUNDMANAGER->release();
	cSoundManager::ReleaseInstance();
	SOUNDDATA->Release();
	cSoundData::ReleaseInstance();

	//����̽� ����
	ReleaseDevice();
}

//������Ʈ
void cMainGame::Update()
{
	//Ÿ�ӸŴ��� ������Ʈ
	TIME_MGR->UpdateTime(60.0f);
	SOUNDMANAGER->update();
	//�������� ���� �ð�
	double timeDelta = TIME_MGR->GetFrameDeltaSec();

	//��������Ʈ
	SCENE_MGR->Update(timeDelta);
//	EVENT_MGR->Update(timeDelta);
	zoom();
}

//��ο�
void cMainGame::Draw()
{
	//���۸� û���Ѵ�
	HRESULT result = Device->Clear(
		0,							//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,						//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,		//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		0xff123434,					//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,						//���̹��۸� û���Ұ� ( 0 ~ 1, 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0							//���ٽ� ���۸� ä�ﰪ
		);

	//���� û�Ұ� �����ߴٸ�.......
	if (SUCCEEDED(result))
	{
		//����̽� ������ ���� ���
		Device->BeginScene();

		//������ ���

		//������
		SCENE_MGR->Render();

#ifdef _DEBUG	//����� ��忡���� ����
		//����̽� ������ ���� ���
		//���� �׸���
		//GIZMO_MGR->WorldGrid(1, 10);

		//Ÿ������ ���
		TIME_MGR->DrawTimeInfo();
#endif

		//����̽� ������ ���� ���
		Device->EndScene();

		//�������� ���۳����� ȭ�鿡 ����
		Device->Present(0, 0, 0, 0);
	}

}

//////////////////////////////////////////////////////////////////////////



//���� ���ν��� �Լ�
LRESULT cMainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_MOUSEWHEEL:
		if (bCamUp == true)
		{
			zDelta = (short)HIWORD(wParam);
			//* Xmas
			switch (zDelta)
			{
			case 120:

				if (532 <= zoombarx && zoombarx <= 744)
				{
					zoomin = true;
					zoomout = false;
				}
				break;
			case -120:
				if (532 <= zoombarx || zoombarx <= 744)
				{
					zoomout = true;
					zoomin = false;
				}
				break;
			}
		}
		else
		{
			g_Czoom = 0.0f;
		}
		break;


		//case WM_MOUSEWHEEL:
		//   zDelta = (short)HIWORD(wParam);
		//   switch (zDelta)
		//   {
		//   case 120:
		//      g_Czoom -= 0.2f;
		//      if (g_Czoom <= -0.8f)
		//      {
		//         g_Czoom = -0.8f;
		//      }
		//      break;
		//   case -120:
		//      g_Czoom += 0.2f;
		//      if (g_Czoom >= 0.1f)
		//      {
		//         g_Czoom = 0.1f;
		//      }
		//      break;
		//   }
		//   break;

	case WM_DESTROY:         //�����찡 �ı��ȴٸ�..
		PostQuitMessage(0);   //���α׷� ���� ��û ( �޽��� ������ ���������� �ȴ� )
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}



void cMainGame::zoom()
{
	if (zoomin)
	{
		if (zoombarx >= 640 && zoombarx < 694)
		{
			g_Czoom -= 0.025f;
			zoombarx += 10;
			if (zoombarx >= 694)
			{
				zoombarx = 694;
				zoomin = false;
			}
		}
		if (zoombarx >= 694 && zoombarx < 744)
		{
			g_Czoom -= 0.025f;
			zoombarx += 10;
			if (zoombarx >= 744)
			{
				zoombarx = 744;
				zoomin = false;
			}
		}
		if (zoombarx >= 532 && zoombarx < 584)
		{
			g_Czoom -= 0.025f;
			zoombarx += 10;
			if (zoombarx >= 584)
			{
				zoombarx = 584;
				zoomin = false;
			}
		}
		if (zoombarx >= 584 && zoombarx < 640)
		{
			g_Czoom -= 0.025f;
			zoombarx += 10;
			if (zoombarx >= 640)
			{
				zoombarx = 640;
				zoomin = false;
			}
		}
	}
	if (zoomout)
	{
		if (zoombarx > 694 && zoombarx <= 744)
		{
			g_Czoom += 0.025f;
			zoombarx -= 10;
			if (zoombarx <= 694)
			{
				zoombarx = 694;
				zoomout = false;
			}
		}
		if (zoombarx > 640 && zoombarx <= 694)
		{
			g_Czoom += 0.025f;
			zoombarx -= 10;
			if (zoombarx <= 640)
			{
				zoombarx = 640;
				zoomout = false;
			}
		}
		if (zoombarx <= 640 && zoombarx > 585)
		{
			g_Czoom += 0.025f;
			zoombarx -= 10;
			if (zoombarx <= 585)
			{
				zoombarx = 585;
				zoomout = false;
			}
		}
		if (zoombarx <= 585 && zoombarx > 532)
		{
			g_Czoom += 0.025f;
			zoombarx -= 10;
			if (zoombarx <= 532)
			{
				zoombarx = 532;
				zoomout = false;
			}
		}
	}
}