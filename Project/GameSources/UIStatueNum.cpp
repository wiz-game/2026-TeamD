/*!
@file SoilWall.cpp
@brief 綺麗にしたあわパカ像
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	UIStatueNum::UIStatueNum(const shared_ptr<Stage>& stage, const STRUCT_UIParam& uiParam)
		:UIBase(stage, uiParam) {}

	void UIStatueNum::OnCreate()
	{
		UIBase::OnCreate();
	}

	void UIStatueNum::OnUpdate()
	{
		UIBase::OnUpdate();

		int statuenum = GameManager::Instance().GetDirtNum();

		auto draw = GetComponent<PCTSpriteDraw>(false);

		if (m_statuenum < statuenum)
		{
			//SetActive(true);
			draw->SetTextureResource(L"UI_AwaPaka3");
			draw->SetDiffuse(Col4(0.6f, 0.6f, 0.6f, 1.0f));
		}
		else
		{
			draw->SetTextureResource(L"UI_AwaPaka1");
			draw->SetDiffuse(Col4(1.0f));
		}
	}


}