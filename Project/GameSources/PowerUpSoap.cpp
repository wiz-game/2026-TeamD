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
		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"DEFAULT_CUBE");

		m_colObb = AddComponent<CollisionObb>();

		//m_gravity = AddComponent<Gravity>();
	}

	// 更新
	void PowerUpSoap::OnUpdate()
	{
		GetItem(GetThis<PowerUpSoap>());
	}

	// 当たり判定
}
//end basecross
