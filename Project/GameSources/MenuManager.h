/*!
@file MenuManager.h
@brief メニューマネージャー
@copyright 田村 鳳暉
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class MenuManager
	{
	private:

		MenuManager()
		{
		}
		virtual ~MenuManager() {}
		void SetAllUpdateActive(const bool& isUpdateActive);

	public:
		static MenuManager& Instance();
		void Pause();
		void ClosePause();
	};
}