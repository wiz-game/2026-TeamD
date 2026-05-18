#include "stdafx.h"
#include "Project.h"
#include "UIBlinking.h"

namespace basecross
{
	UIBlinking::UIBlinking
	(
		const shared_ptr<Stage>& stage, 
		const STRUCT_UIParam& uiParam,
		const float& blinkSpeed
	) :
		UIBase(stage, uiParam),
		m_blinkSpeed(blinkSpeed),
		m_blinkAlpha(0.0f),
		m_isIncreasing(true)
	{
	}

	void UIBlinking::OnCreate()
	{
		UIBase::OnCreate();
	}

	void UIBlinking::OnUpdate()
	{
		UIBase::OnUpdate();
		UpdateBlinking();
	}

	void UIBlinking::UpdateBlinking()
	{
		auto newBlink = m_blinkSpeed * App::GetApp()->GetElapsedTime();

		if (m_isIncreasing)
		{
			m_blinkAlpha += sinf(newBlink);
			if (m_blinkAlpha > 1.0f)
			{
				m_blinkAlpha = 1.0f;
				m_isIncreasing = false;
			}
		}
		else
		{
			m_blinkAlpha -= sinf(newBlink);
			if (m_blinkAlpha < 0.0f)
			{
				m_blinkAlpha = 0.0f;
				m_isIncreasing = true;
			}
		}
		
		m_sPtrDraw->SetDiffuse(Col4(m_color.x, m_color.y, m_color.z, m_blinkAlpha));
	}
}