/*!
@file MenuManager.h
@brief メニューマネージャー
@copyright 田村 鳳暉
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	enum class ENUM_ClearMode
	{
		Retry,
		Retitle
	};

	enum class ENUM_GameOverMode
	{
		Retry,
		Retitle
	};

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
		Return,
		Reset
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
		vector<shared_ptr<UIBase>> m_uisounds;

		vector<shared_ptr<UIBase>> m_uiclears;
		vector<shared_ptr<UIBase>> m_uigameovers;

		Col4 m_uidiffuse;
		float m_uiscale = 0.0f;
		float m_uiBGMpos = 125.0f;
		float m_uiSEpos = 125.0f;

		ENUM_ClearMode m_clearMode = ENUM_ClearMode::Retry;
		ENUM_GameOverMode m_gameoverMode = ENUM_GameOverMode::Retry;
		ENUM_MenuMode m_menuMode = ENUM_MenuMode::Default;
		ENUM_MenuStart m_menuUI = ENUM_MenuStart::Restart;
		ENUM_Setting m_settingUI = ENUM_Setting::BGM;

		MenuManager()
		{}
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
		void ChangeUISoundsVol(float num);
		void ChangeSelectMenuMode(const int& num);

		void SetUIDiffuse(int i, const vector<shared_ptr<UIBase>>& uipointers, const Col4& diffuse);
		void SetUISize(int i, const vector<shared_ptr<UIBase>>& uipointers, const float& scale);
		void SetUISoundsVol(int i, const vector<shared_ptr<UIBase>>& uipointers, const float& add);

		void SetUIFrames(const vector<shared_ptr<UIBase>>& uiframes);
		void SetUIDefaults(const vector<shared_ptr<UIBase>>& uidefaults);
		void SetUISettings(const vector<shared_ptr<UIBase>>& uisettings);
		void SetUIHowtoplays(const vector<shared_ptr<UIBase>>& uihowtoplays);
		void SetUIDiffuse(const Col4& uidiffuse);
		void SetUIScale(const float& uiscale);

		void SetUIClears(const vector<shared_ptr<UIBase>>& uiclear);
		void SetUIGameOvers(const vector<shared_ptr<UIBase>>& uigameover);

		void SetMenuMode(ENUM_MenuMode menumode);
		void SetMenuUI(ENUM_MenuStart menuui);
		void SetSettingUI(ENUM_Setting settingui);

		void SetClearUI(ENUM_ClearMode clearmode);
		void SetGameOverUI(ENUM_GameOverMode gameovermode);

		void SetBGMPos(float pos);
		void SetSEPos(float pos);
		float GetBGMPos() const { return m_uiBGMpos; }
		float GetSEPos() const { return m_uiSEpos; }

		ENUM_ClearMode GetClearMode() const { return m_clearMode; }
		ENUM_GameOverMode GetGameOverMode() const { return m_gameoverMode; }
		ENUM_MenuMode GetMenuMode() const { return m_menuMode; }
		ENUM_MenuStart GetMenuUI() const { return m_menuUI; }
		ENUM_Setting GetSettingUI() const { return m_settingUI; }
	};
}