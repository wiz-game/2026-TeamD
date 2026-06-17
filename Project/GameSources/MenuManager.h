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
		Restart,
		Setting,
		Howtoplay,
		Retitle
	};

	class MenuManager
	{
	private:
		vector<shared_ptr<UIBase>> m_uipointers;
		ENUM_MenuMode m_menuMode = ENUM_MenuMode::Restart;

		MenuManager()
		{
		}
		virtual ~MenuManager() {}
		void SetAllUpdateActive(const bool& isUpdateActive);

	public:
		static MenuManager& Instance();
		void Pause();
		void ClosePause();
		void UIDrawActive(bool isActive);
		void SetUIPointers(const vector<shared_ptr<UIBase>>& uipointers);

		void SetMenuMode(ENUM_MenuMode menumode);
		ENUM_MenuMode GetMenuMode() const { return m_menuMode; }

		void ChangeSelectMenuMode(const int& num);

	};
}