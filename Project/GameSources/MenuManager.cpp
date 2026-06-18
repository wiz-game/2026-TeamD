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

	void MenuManager::ChangeUIDiffuse(Col4 diffuse)
	{
		Col4 defaultCol = Col4(0.0f, 0.0f, 0.0f, 1.0f);
		Col4 elseCol = Col4(0.8f, 0.8f, 0.8f, 1.0f);

		switch (GetMenuMode())
		{
		case ENUM_MenuMode::Restart:
			m_buttondiffuse = defaultCol;
			break;
		case ENUM_MenuMode::Setting:

			break;
		case ENUM_MenuMode::Howtoplay:

			break;
		case ENUM_MenuMode::Retitle:

			break;

		}
	}

	void MenuManager::SetUIPointers(const vector<shared_ptr<UIBase>>& uipointers)
	{
		m_uipointers = uipointers;
	}

	void MenuManager::SetButtonDiffuse(Col4 diffuse)
	{
		m_buttondiffuse = diffuse;
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