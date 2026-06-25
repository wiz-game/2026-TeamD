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
		SetMenuMode(ENUM_MenuMode::MenuStart);
		ChangeMenuMode();
	}

	void MenuManager::ClosePause()
	{
		SetAllUpdateActive(true);
		SetMenuMode(ENUM_MenuMode::Default);
		ChangeMenuMode();
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

	void MenuManager::UIDrawActive(bool isActive, vector<shared_ptr<UIBase>> uipointers)
	{
		for (auto& uipointer : uipointers)
		{
			uipointer->GetComponent<PCTSpriteDraw>()->SetDrawActive(isActive);
		}
	}

	void MenuManager::ChangeUIDiffuse()
	{
		Col4 defaultCol = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		Col4 elseCol = Col4(0.7f, 0.7f, 0.7f, 1.0f);

		Col4 defaultEmi = Col4(0.0f);
		Col4 elseEmi = Col4(0.0f, 0.0f, 0.0f, 0.0f);

		switch (GetMenuUI())
		{
		case ENUM_MenuStart::Restart:
			SetButtonDiffuse(0, defaultCol, defaultEmi);
			SetButtonDiffuse(1, elseCol, elseEmi);
			SetButtonDiffuse(2, elseCol, elseEmi);
			SetButtonDiffuse(3, elseCol, elseEmi);
			break;
		case ENUM_MenuStart::Setting:
			SetButtonDiffuse(0, elseCol, elseEmi);
			SetButtonDiffuse(1, defaultCol, defaultEmi);
			SetButtonDiffuse(2, elseCol, elseEmi);
			SetButtonDiffuse(3, elseCol, elseEmi);
			break;
		case ENUM_MenuStart::Howtoplay:
			SetButtonDiffuse(0, elseCol, elseEmi);
			SetButtonDiffuse(1, elseCol, elseEmi);
			SetButtonDiffuse(2, defaultCol, defaultEmi);
			SetButtonDiffuse(3, elseCol, elseEmi);
			break;
		case ENUM_MenuStart::Retitle:
			SetButtonDiffuse(0, elseCol, elseEmi);
			SetButtonDiffuse(1, elseCol, elseEmi);
			SetButtonDiffuse(2, elseCol, elseEmi);
			SetButtonDiffuse(3, defaultCol, defaultEmi);
			break;
		}
	}

	void MenuManager::ChangeMenuMode()
	{
		switch (m_menuMode)
		{
		case ENUM_MenuMode::Default:
			UIDrawActive(false, m_uidefaults);
			UIDrawActive(false, m_uiframes);
			UIDrawActive(false, m_uisettings);
			UIDrawActive(false, m_uihowtoplays);
			break;

		case ENUM_MenuMode::MenuStart:
			UIDrawActive(true, m_uiframes);
			UIDrawActive(true, m_uidefaults);
			UIDrawActive(false, m_uisettings);
			UIDrawActive(false, m_uihowtoplays);
			break;

		case ENUM_MenuMode::Setting:
			UIDrawActive(true, m_uiframes);
			UIDrawActive(false, m_uidefaults);
			UIDrawActive(true, m_uisettings);
			UIDrawActive(false, m_uihowtoplays);
			break;

		case ENUM_MenuMode::Howtoplay:
			UIDrawActive(true, m_uiframes);
			UIDrawActive(false, m_uidefaults);
			UIDrawActive(false, m_uisettings);
			UIDrawActive(true, m_uihowtoplays);
			break;
		}
	}

	void MenuManager::SetUIFrames(const vector<shared_ptr<UIBase>>& uiframes)
	{
		m_uiframes = uiframes;
	}

	void MenuManager::SetUIDefaults(const vector<shared_ptr<UIBase>>& uidefaults)
	{
		m_uidefaults = uidefaults;
	}

	void MenuManager::SetUISettings(const vector<shared_ptr<UIBase>>& uisettings)
	{
		m_uisettings = uisettings;
	}

	void MenuManager::SetUIHowtoplays(const vector<shared_ptr<UIBase>>& uihowtoplays)
	{
		m_uihowtoplays = uihowtoplays;
	}

	void MenuManager::SetMenuMode(ENUM_MenuMode menumode)
	{
		m_menuMode = menumode;
	}

	void MenuManager::SetButtonDiffuse(int i, const Col4& diffuse, Col4& emissive)
	{
		if (i < 0 || i >= m_uidefaults.size())return;

		auto ui = m_uidefaults[i];
		if (!ui)return;

		auto draw = ui->GetComponent<PCTSpriteDraw>();
		if (ui)
		{
			draw->SetDiffuse(diffuse);
			draw->SetEmissive(emissive);
		}
	}
	
	void basecross::MenuManager::SetMenuUI(ENUM_MenuStart menuui)
	{
		m_menuUI = menuui;
	}

	void MenuManager::ChangeSelectMenuMode(const int& num)
	{
		auto menuNow = GetMenuUI();
		int menuAfter = static_cast<int>(menuNow);
		menuAfter += num;

		if (menuAfter == -1) menuAfter = 3;
		if (menuAfter == 4) menuAfter = 0;

		ENUM_MenuStart setAfter = static_cast<ENUM_MenuStart>(menuAfter); 

		SetMenuUI(setAfter);
	}
}