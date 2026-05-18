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
			GameManager::Instance().AddDebugStr(L"Stage", L"SelectStage");
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
