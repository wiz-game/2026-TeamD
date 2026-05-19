/*!
@file SelectStage.cpp
@brief セレクトステージ
@Copyright 菅野 皐
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void SelectStage::OnCreate()
	{
		try
		{
			CreateView<SingleView>();
			AddGameObject<UITransitionSlide>(STRUCT_UIParam(L"Awas", Vec3(0.0f, 0.0f, 0.0f), 1.3f), 600.0f);
		}
		catch (...)
		{
			throw;
		}
	}

	void SelectStage::OnUpdate()
	{
	}
}
