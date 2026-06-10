#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	UIMenu::UIMenu(const shared_ptr<Stage>& stage, const STRUCT_UIParam& uiParam)
	:UIBase(stage,uiParam){}

	void UIMenu::OnCreate()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();

		auto backscreen = stage->AddGameObject<UIBase>(STRUCT_UIParam(L"MenuBack", Vec3(0.0f, 0.0f, 0.0f), 1.0f, Col4(1.0f, 1.0f, 1.0f, 0.25f)));
		m_backscreen = backscreen;


	}

	void UIMenu::OnUpdate()
	{

	}

	void UIMenu::UIDrawActive(bool isActive)
	{
		//メニューの後ろの画面をぼかすやつ
		auto backscreen = m_backscreen.lock();
		m_backscreen = backscreen;
		backscreen->SetDrawActive(isActive);
	}

}