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
		m_transform->SetScale(0.5f, 1.0f, 0.5f);

		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"DEFAULT_CUBE");

		m_colObb = AddComponent<CollisionObb>();

		//m_gravity = AddComponent<Gravity>();
	}

	// 更新
	void PowerUpSoap::OnUpdate()
	{
	}

}
//end basecross
