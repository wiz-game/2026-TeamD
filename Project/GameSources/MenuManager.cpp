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

	void MenuManager::ChangeUIDiffuse()
	{
		Col4 defaultCol = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		Col4 elseCol = Col4(0.6f, 0.6f, 0.6f, 1.0f);

		switch (GetMenuMode())
		{
		case ENUM_MenuMode::Restart:
			SetButtonDiffuse(2, defaultCol);
			SetButtonDiffuse(3, elseCol);
			SetButtonDiffuse(4, elseCol);
			SetButtonDiffuse(5, elseCol);
			break;
		case ENUM_MenuMode::Setting:
			SetButtonDiffuse(2, elseCol);
			SetButtonDiffuse(3, defaultCol);
			SetButtonDiffuse(4, elseCol);
			SetButtonDiffuse(5, elseCol);
			break;
		case ENUM_MenuMode::Howtoplay:
			SetButtonDiffuse(2, elseCol);
			SetButtonDiffuse(3, elseCol);
			SetButtonDiffuse(4, defaultCol);
			SetButtonDiffuse(5, elseCol);
			break;
		case ENUM_MenuMode::Retitle:
			SetButtonDiffuse(2, elseCol);
			SetButtonDiffuse(3, elseCol);
			SetButtonDiffuse(4, elseCol);
			SetButtonDiffuse(5, defaultCol);
			break;
		}
	}

	void MenuManager::SetUIPointers(const vector<shared_ptr<UIBase>>& uipointers)
	{
		m_uipointers = uipointers;
	}

	void MenuManager::SetButtonDiffuse(int i, const Col4& diffuse)
	{
		if (i < 0 || i >= m_uipointers.size())return;

		auto ui = m_uipointers[i];
		if (!ui)return;

		auto draw = ui->GetComponent<PCTSpriteDraw>();
		if (ui)
		{
			draw->SetDiffuse(diffuse);
		}
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