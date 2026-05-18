/*!
@file TitleStage.cpp
@brief タイトルステージ
@Copyright 菅野 皐
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void TitleStage::OnCreate()
	{
		try
		{
			CreateView<SingleView>();
			AddGameObject<UIBase>(L"awapaka_logo", Vec3(0.0f, 120.0f, 0.0f));
		}
		catch (...) 
		{
			throw;
		}
	}

	void TitleStage::OnUpdate()
	{
		GameManager::Instance().AddDebugStr(L"Stage", L"TitleStage");
		GameManager::Instance().AddDebugStr(L"StartMessage", L"Press A Button");
	}
}
