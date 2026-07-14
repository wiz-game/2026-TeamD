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
	}

	void UIStatueNum::ChangeStatueActive(int uinum, const vector<shared_ptr<UIStatueNum>>& uipointers, bool isActive)
	{
		if (uinum < 0 || uinum >= uipointers.size()) return;

		auto ui = uipointers[uinum];
		if (!ui) return;

		auto draw = ui->GetComponent<PCTSpriteDraw>();

		if (isActive)
		{
			draw->SetDrawActive(isActive);
		}
		else
		{

		}

	}

}