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
			
			float imageSizeRatio = 0.6f;
			AddGameObject<UIBase>(L"awapaka_logo", Vec3(0.0f, 120.0f, 0.0f), Vec2(1920, 1080) * imageSizeRatio);

			imageSizeRatio = 0.4f;
			AddGameObject<UIBlinking>(L"StartButton", Vec3(0.0f, -200.0f, 0.0f), 0.8f, Vec2(1920, 1080) * imageSizeRatio);
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
