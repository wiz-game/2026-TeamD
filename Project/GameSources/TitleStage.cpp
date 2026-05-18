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
