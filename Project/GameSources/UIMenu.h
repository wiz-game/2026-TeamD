/*!
@file UIMenu.h
@brief 
@copyright  
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class UIMenu : public UIBase
	{
	private:

	public:
		UIMenu(const shared_ptr<Stage>& stage, const STRUCT_UIParam& uiParam);
		virtual ~UIMenu(){}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

}