/*!
@file MenuManager.h
@brief メニューマネージャー
@copyright 田村 鳳暉
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	enum class ENUM_MenuMode
	{
		Default,
		MenuStart,
		Setting,
		Howtoplay
	};

	enum class ENUM_MenuStart
	{
		Restart,
		Setting,
		Howtoplay,
		Retitle
	};

	enum class ENUM_Setting
	{
		BGM,
		SE,
		Return
	};

	enum class ENUM_Howtoplay
	{
		Return
	};


	class MenuManager
	{
	private:
		vector<shared_ptr<UIBase>> m_uiframes;
		vector<shared_ptr<UIBase>> m_uidefaults;
		vector<shared_ptr<UIBase>> m_uisettings;
		vector<shared_ptr<UIBase>> m_uihowtoplays;

		Col4 m_uidiffuse;
		float m_uiscale;

		ENUM_MenuStart m_menuUI = ENUM_MenuStart::Restart;
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

		void UIFrameDrawActive(bool isActive);
		void UIDrawActive(bool isActive, vector<shared_ptr<UIBase>> uipointers);

		void ChangeMenuMode();
		void ChangeUISize(float size);
		void ChangeUIParam();
		void ChangeSelectMenuMode(const int& num);

		void SetUIDiffuse(int i, const vector<shared_ptr<UIBase>>&uipointers, const Col4& diffuse);
		void SetUISize(int i, const vector<shared_ptr<UIBase>>& uipointers, const float& scale);

		void SetUIFrames(const vector<shared_ptr<UIBase>>& uiframes);
		void SetUIDefaults(const vector<shared_ptr<UIBase>>& uidefaults);
		void SetUISettings(const vector<shared_ptr<UIBase>>& uisettings);
		void SetUIHowtoplays(const vector<shared_ptr<UIBase>>& uihowtoplays);
		void SetUIDiffuse(const Col4& uidiffuse);
		void SetUIScale(const float& uiscale);
		void SetMenuMode(ENUM_MenuMode menumode);
		void SetMenuUI(ENUM_MenuStart menuui);

		ENUM_MenuStart GetMenuUI() const { return m_menuUI; }
		ENUM_MenuMode GetMenuMode() const { return m_menuMode; }


	};
}