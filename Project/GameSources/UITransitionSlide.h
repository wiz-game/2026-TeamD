/*!
@file UITransitionSlide.h
@brief 画面遷移用のスライドするUI
@Copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class UITransitionSlide : public UISlide
	{
	private:
		bool m_isSlideUp;
		float m_destroyY;
		void UpdateTransition();
	public:
		UITransitionSlide
		(
			const shared_ptr<Stage>& stage,
			const STRUCT_UIParam& uiParam,
			const float& slideSpeed = 1.0f,
			const bool& isSlideUp = false
		);
		virtual ~UITransitionSlide() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}
