/*!
@file GameClearStage.cpp
@brief ゲームクリアステージ
@Copyright 菅野 皐
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void GameClearStage::OnCreate()
	{
		try
		{
			CreateView<SingleView>();
			AddGameObject<UIBase>(STRUCT_UIParam(L"GameClear", Vec3(0.0f, 140.0f, 0.0f), 0.7f));
			AddGameObject<UIBase>(STRUCT_UIParam(L"RetrunA", Vec3(0.0f, -140.0f, 0.0f), 0.2f));
			AddGameObject<UIBase>(STRUCT_UIParam(L"TitleModoru", Vec3(0.0f, -260.0f, 0.0f), 0.2f));
		}
		catch (...)
		{
			throw;
		}
	}

	void GameClearStage::OnUpdate()
	{
	}
}
