/*!
@file GameOverStage.cpp
@brief ゲームオーバーステージ
@Copyright 菅野 皐
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void GameOverStage::OnCreate()
	{
		try
		{
			CreateView<SingleView>();
			AddGameObject<UIBase>(STRUCT_UIParam(L"GameOver", Vec3(0.0f, 140.0f, 0.0f), 0.7f));
			AddGameObject<UIBase>(STRUCT_UIParam(L"RetrunA", Vec3(0.0f, -140.0f, 0.0f), 0.2f));
			AddGameObject<UIBase>(STRUCT_UIParam(L"TitleModoru", Vec3(0.0f, -260.0f, 0.0f), 0.2f));

		}
		catch (...)
		{
			throw;
		}
	}

	void GameOverStage::OnUpdate()
	{
	}
}
