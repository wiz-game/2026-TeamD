/*!
@file MenuManager.h
@brief メニューマネージャー
@copyright 田村 鳳暉
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	enum class ENUM_MenuUI
	{
		Restart,
		Setting,
		Howtoplay,
		Retitle
	};

	enum class ENUM_MenuMode
	{
		Default,
		MenuStart,
		Setting,
		Howtoplay
	};

	class MenuManager
	{
	private:
		vector<shared_ptr<UIBase>> m_uiframes;
		vector<shared_ptr<UIBase>> m_uidefaults;
		vector<shared_ptr<UIBase>> m_uisettings;
		vector<shared_ptr<UIBase>> m_uihowtoplays;

		ENUM_MenuUI m_menuUI = ENUM_MenuUI::Restart;
		ENUM_MenuMode m_menuMode = ENUM_MenuMode::Default;

		MenuManager()
		{
		}
		virtual ~MenuManager() {}
		void SetAllUpdateActive(const bool& isUpdateActive);

	public:
		static MenuManager& Instance();
		void Pause();
		void ClosePause();
		void ChangeUIDiffuse();
		void ChangeMenuMode();

		void UIFrameDrawActive(bool isActive);
		void UIDrawActive(bool isActive, vector<shared_ptr<UIBase>> uipointers);

		void SetUIFrames(const vector<shared_ptr<UIBase>>& uiframes);
		void SetUIDefaults(const vector<shared_ptr<UIBase>>& uidefaults);
		void SetUISettings(const vector<shared_ptr<UIBase>>& uisettings);
		void SetUIHowtoplays(const vector<shared_ptr<UIBase>>& uihowtoplays);

		void SetButtonDiffuse(int i, const Col4& diffuse, Col4& emissive);

		void SetMenuMode(ENUM_MenuMode menumode);
		void SetMenuUI(ENUM_MenuUI menuui);
		ENUM_MenuUI GetMenuUI() const { return m_menuUI; }
		ENUM_MenuMode GetMenuMode() const { return m_menuMode; }

		void ChangeSelectMenuMode(const int& num);

	};
}