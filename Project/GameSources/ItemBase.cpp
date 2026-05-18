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

	void ItemBase::GetSoapOfCoolDown(const shared_ptr<GameObject>& gameObject)
	{

	}

	void ItemBase::GetItem(const shared_ptr<GameObject>& gameObject)
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		stage->RemoveGameObject<GameObject>(GetThis<GameObject>());
	}

	// 当たり判定
	void ItemBase::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
	}

	void ItemBase::OnCollisionExecute(shared_ptr<GameObject>& Other)
	{
	}

	void ItemBase::OnCollisionExit(shared_ptr<GameObject>& Other)
	{

	}
}
//end basecross
