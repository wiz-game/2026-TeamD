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
		}
		catch (...)
		{
			throw;
		}
	}

	void SelectStage::OnUpdate()
	{
		GameManager::Instance().AddDebugStr(L"Stage", L"SelectStage");
		GameManager::Instance().AddDebugStr(L"SelectedStage", GameManager::Instance().GetSelectGameStage());
	}
}
