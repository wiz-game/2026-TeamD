#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	UIMenu::UIMenu(const shared_ptr<Stage>& stage, const STRUCT_UIParam& uiParam)
	:UIBase(stage,uiParam){}

	void UIMenu::OnCreate()
	{
		UIBase::OnCreate();
	}

	void UIMenu::OnUpdate()
	{
		UIBase::OnUpdate();
		UIDrawActive(true);
	}

	void UIMenu::UIDrawActive(bool isActive)
	{
		//メニューの後ろの画面をぼかすやつ
		auto backscreen = m_backscreen.lock();
		m_backscreen = backscreen;
		backscreen->SetDrawActive(isActive);
	}

}