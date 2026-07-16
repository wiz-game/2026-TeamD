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
			AddGameObject<UIBase>(STRUCT_UIParam(L"UI_GameClear", Vec3(0.0f, 140.0f, 0.0f), 0.7f));
			AddGameObject<UIBase>(STRUCT_UIParam(L"UI_OptionButton_5", Vec3(0.0f, -140.0f, 0.0f), 0.25f));
			AddGameObject<UIBase>(STRUCT_UIParam(L"UI_OptionButton_4", Vec3(0.0f, -260.0f, 0.0f), 0.25f));
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
