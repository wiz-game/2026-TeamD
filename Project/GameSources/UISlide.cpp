#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	UISlide::UISlide(const shared_ptr<Stage>& stage, const STRUCT_UIParam& uiParam, const float& slideSpeed)
		: UIBase(stage, uiParam),
		m_slideSpeed(slideSpeed)
	{
	}

	void UISlide::OnCreate()
	{
		UIBase::OnCreate();
	}

	void UISlide::OnUpdate()
	{
		UIBase::OnUpdate();
		UpdateSlide();
	}

	void UISlide::UpdateSlide()
	{
		auto elapsedTime = App::GetApp()->GetElapsedTime();
		auto newPos = m_sPtrTrans->GetPosition();
		newPos.y += m_slideSpeed * elapsedTime;
		m_sPtrTrans->SetPosition(newPos);
	}
}