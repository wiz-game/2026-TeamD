#include "stdafx.h"
#include "Project.h"
#include "MenuManager.h"

namespace basecross
{
	MenuManager& MenuManager::Instance()
	{
		static MenuManager instance;
		return instance;
	}

	void MenuManager::Pause()
	{
		SetAllUpdateActive(false);
		UIDrawActive(true);

	}

	void MenuManager::ClosePause()
	{
		SetAllUpdateActive(true);
		UIDrawActive(false);
	}

	//ステージの全オブジェクトのUpdateを管理する
	void MenuManager::SetAllUpdateActive(const bool& isUpdateActive)
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto gameObjects = stage->GetGameObjectVec();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetUpdateActive(isUpdateActive);
		}
	}

	void MenuManager::UIDrawActive(bool isActive)
	{
		for (auto& uipointer : m_uipointers)
		{
			uipointer->GetComponent<PCTSpriteDraw>()->SetDrawActive(isActive);
		}
	}

	void MenuManager::SetUIPointers(const vector<shared_ptr<UIBase>>& uipointers)
	{
		m_uipointers = uipointers;
	}
	
	void basecross::MenuManager::SetMenuMode(ENUM_MenuMode menumode)
	{
		m_menuMode = menumode;
	}

	void MenuManager::ChangeSelectMenuMode(const int& num)
	{
		auto menuNow = GetMenuMode();
		int menuAfter = static_cast<int>(menuNow);
		menuAfter += num;

		if (menuAfter == -1) menuAfter = 3;
		if (menuAfter == 4) menuAfter = 0;

		ENUM_MenuMode setAfter = static_cast<ENUM_MenuMode>(menuAfter); 

		SetMenuMode(setAfter);
	}
}