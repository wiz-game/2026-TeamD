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
		std::weak_ptr<UIBase>m_backscreen;
	public:
		UIMenu(const shared_ptr<Stage>& stage, const STRUCT_UIParam& uiParam);
		virtual ~UIMenu(){}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void UIDrawActive(bool isActive);
	};

}