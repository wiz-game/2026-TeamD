#include "stdafx.h"
#include "Project.h"
#include "UIBlinking.h"

namespace basecross
{
	UIBlinking::UIBlinking
	(
		const shared_ptr<Stage>& stage, 
		const wstring& meshName, 
		const Vec3& position, 
		const float& blinkSpeed, 
		const Vec2& imageSize
	) :
		UIBase(stage, meshName, position, imageSize),
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