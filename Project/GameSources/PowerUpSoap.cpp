/*!
@file PowerUpSoap.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{

	//初期化
	void PowerUpSoap::OnCreate()
	{
		AddTag(L"Soap");

		m_transform = AddComponent<Transform>();
		m_transform->SetScale(m_Scale);
		m_transform->SetRotation(m_Rotation);

		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"DEFAULT_CUBE");

		m_colObb = AddComponent<CollisionObb>();

		m_InitPosition = m_Position;
	}

	// 更新
	void PowerUpSoap::OnUpdate()
	{
		Rotation();
		UpDown();
		DebugStr();
	}

	void PowerUpSoap::Rotation()
	{
		m_Rotation.y += m_RotationSpeed * App::GetApp()->GetElapsedTime();
		m_transform->SetRotation(m_Rotation.x,m_Rotation.y,m_Rotation.z);
	}

	void PowerUpSoap::UpDown()
	{
		float upDown = 0.5;
		if (m_isUp == true)
		{
			m_Position.y += m_UpSpeed * App::GetApp()->GetElapsedTime();
			if (m_Position.y >= m_InitPosition.y + upDown)
			{
				m_isUp = false;
			}
		}
		else 
		{
			m_Position.y -= m_UpSpeed * App::GetApp()->GetElapsedTime();
			if (m_Position.y <= m_InitPosition.y - upDown)
			{
				m_isUp = true;
			}
		}
		m_transform->SetPosition(m_Position.x,m_Position.y,m_Position.z);
	}

	void PowerUpSoap::DebugStr()
	{
		auto transRot = m_transform->GetRotation();
	
		GameManager::Instance().AddDebugStr(L"Rotation_Y", transRot.y);
	}
}
//end basecross
