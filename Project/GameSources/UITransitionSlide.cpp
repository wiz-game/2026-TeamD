#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	UITransitionSlide::UITransitionSlide(const shared_ptr<Stage>& stage, const STRUCT_UIParam& uiParam, const float& slideSpeed, const bool& isSlideUp)
		: UISlide(stage, uiParam, slideSpeed),
		m_isSlideUp(isSlideUp),
		m_destroyY(4000.0f)
	{
	}
	
	void UITransitionSlide::OnCreate()
	{
		UISlide::OnCreate();
	}

	void UITransitionSlide::OnUpdate()
	{
		UISlide::OnUpdate();
		UpdateTransition();
	}

	void UITransitionSlide::UpdateTransition()
	{
		auto nowPos = m_sPtrTrans->GetPosition();
		if (m_isSlideUp)
		{
			if (nowPos.y >= 0.0f)
			{
				m_sPtrTrans->SetPosition(Vec3(nowPos.x, 0.0f, nowPos.z));
				SetSlideSpeed(0.0f);
				GameManager::Instance().SetGameMode(GameManager::Instance().GetTransitionAfterGameMode());
				GameManager::Instance().ResetTransitionAfterGameMode();
			}
		}
		else
		{
			if (nowPos.y >= m_destroyY)
			{
				GetStage()->RemoveGameObject<UITransitionSlide>(GetThis<UITransitionSlide>());
			}
		}
	}
}
