/*!
@file UIBlinking.h
@brief 点滅するUI
@Copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class UIBlinking : public UIBase
	{
	private:
		float m_blinkSpeed;
		float m_blinkAlpha;
		bool m_isIncreasing;

		void UpdateBlinking();
	public:
		UIBlinking
		(
			const shared_ptr<Stage>& stage, 
			const STRUCT_UIParam& uiParam,
			const float& blinkSpeed = 1.0f
		);
		virtual ~UIBlinking() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		float GetBlinkSpeed() const { return m_blinkSpeed; }
		void SetBlinkSpeed(const float& blinkSpeed) { m_blinkSpeed = blinkSpeed; }
	};
}
