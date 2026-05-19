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

	void PowerUpSoap::GetSoapOfCoolDown()
	{
		// 現在有効なステージの情報を取得する
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (stage == nullptr)	// エラー対策
		{
			return;
		}

		// Playerの情報を取得する
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		if (player == nullptr)	// エラー対策
		{
			return;
		}

		player->SetCoolDown(true);
	}
}
//end basecross
