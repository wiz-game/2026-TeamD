/*!
@file UISlide.h
@brief スライドするUI
@Copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class UISlide : public UIBase
	{
	private:
		float m_slideSpeed;
		void UpdateSlide();
	public:
		UISlide
		(
			const shared_ptr<Stage>& stage,
			const STRUCT_UIParam& uiParam,
			const float& slideSpeed = 1.0f
		);
		virtual ~UISlide() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		float GetSlideSpeed() const { return m_slideSpeed; }
		void SetSlideSpeed(const float& slideSpeed) { m_slideSpeed = slideSpeed; }
	};
}
