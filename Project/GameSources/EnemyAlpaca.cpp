/*!
@file EnemyAlpaca.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{

	// 初期化
	void EnemyAlpaca::OnCreate()
	{
		AddTag(L"Enemy");
		auto transComp = AddComponent<Transform>();
		transComp->SetPosition(m_Position);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"M_Alpaca");
		drawComp->SetTextureResource(L"T_Alpaca");
		drawComp->SetDrawActive(true);

		auto obb = AddComponent<CollisionObb>();
	}

	// 更新
	void EnemyAlpaca::OnUpdate()
	{
		PointMove(GetThis<EnemyAlpaca>(), m_Speed);
	}

}
//end basecross
