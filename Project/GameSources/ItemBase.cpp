/*!
@file ItemBase.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{

	//初期化
	void ItemBase::OnCreate()
	{

	}

	// 更新
	void ItemBase::OnUpdate()
	{

	}

	void ItemBase::GetItem(const shared_ptr<GameObject>& gameObject)
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		stage->RemoveGameObject<GameObject>(GetThis<GameObject>());
	}

	void ItemBase::GetSoapOfCoolDown()
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

	}

	// 当たり判定
	void ItemBase::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		// 現在有効なステージの情報を取得する
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();

		// Playerの情報を取得する
		auto player = stage->GetSharedGameObject<Player>(L"Player");

		if (Other->FindTag(L"Player"))
		{
			GetItem(GetThis<GameObject>());
			player->SetCoolDown(true);
		}
	}

	void ItemBase::OnCollisionExecute(shared_ptr<GameObject>& Other)
	{
	}

	void ItemBase::OnCollisionExit(shared_ptr<GameObject>& Other)
	{

	}
}
//end basecross
