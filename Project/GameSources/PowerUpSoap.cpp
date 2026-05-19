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
		m_transform->SetRotation(0.0f, 0.0f, XMConvertToRadians(45.0f));

		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"DEFAULT_CUBE");

		m_colObb = AddComponent<CollisionObb>();

		//m_gravity = AddComponent<Gravity>();
	}

	// 更新
	void PowerUpSoap::OnUpdate()
	{
		Rotation();
		DebugStr();
	}

	void PowerUpSoap::Rotation()
	{
		auto transRot = m_transform->GetRotation();
		transRot.y += m_RotationSpeed * App::GetApp()->GetElapsedTime();
		m_transform->SetRotation(transRot);
	}

	void PowerUpSoap::UpDown()
	{
		auto transPos = m_transform->GetPosition();
		transPos.y += App::GetApp()->GetElapsedTime();
		//m_transform->SetPosition()
	}

	void PowerUpSoap::DebugStr()
	{
		auto transRot = m_transform->GetRotation();
	
		GameManager::Instance().AddDebugStr(L"Rotation_Y", transRot.y);
	}
}
//end basecross
