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
			AddGameObject<UIBase>(STRUCT_UIParam(L"awapaka_logo", Vec3(0.0f, 120.0f, 0.0f), 0.6f));
			auto startButton = AddGameObject<UIBlinking>(STRUCT_UIParam(L"StartButton", Vec3(0.0f, -200.0f, 0.0f), 0.4f), 0.8f);
			SetSharedGameObject(L"StartButton", startButton);
			AddGameObject<UITransitionSlide>(STRUCT_UIParam(L"Awas", Vec3(0.0f, 0.0f, 0.0f), 1.3f), 600.0f);
		}
		catch (...) 
		{
			throw;
		}
	}

	void TitleStage::OnUpdate()
	{
	}
}
