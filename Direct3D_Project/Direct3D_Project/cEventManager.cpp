#include "stdafx.h"
#include "cEventManager.h"


cEventManager::cEventManager()
{
}


cEventManager::~cEventManager()
{
}

void cEventManager::release()
{
}

void cEventManager::update()
{
	for (int i = 0; i < m_vEventBox.size(); i++)
	{
		if (m_vEventBox[i]->getActive())
		{
			m_vEventBox[i]->EventUpdate();
		}

		if (m_vEventBox[i]->CollisionCheck())
		{
			if (m_vEventBox[i]->getCollisionActive())
			{
				m_vEventBox[i]->EventCall();
				m_vEventBox[i]->setCollisionActive(false);
				m_vEventBox[i]->setActive(true);
			}
			
			break;
		}
	}
}

void cEventManager::PushEventBox(cEventBox * Eventbox)
{
	m_vEventBox.push_back(Eventbox);
}

void cEventManager::Render()
{
	for (int i = 0; i < m_vEventBox.size(); i++)
	{
		if (m_vEventBox[i]->getActive())
		{
			m_vEventBox[i]->render();
		}
	}
}
