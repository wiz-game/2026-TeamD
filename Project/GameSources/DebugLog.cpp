/*!
@file DebugLog.h
@brief デバッグ用文字列を表示、管理する
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	DebugLog::DebugLog
	(
		const shared_ptr<Stage>& stagePtr,
		const wstring& logTitle
	) :
		GameObject(stagePtr)
	{
		m_logTitle = logTitle;
		m_debugStr = m_logTitle;
	}

	void DebugLog::OnCreate() 
	{
		m_sPtrStrComp = AddComponent<StringSprite>();

		m_sPtrStrComp->SetBackColor(Col4(0.0f, 0.0f, 0.0f, 0.8f));
		m_sPtrStrComp->SetFontColor(Col4(0.0f, 1.0f, 0.0f, 1.0f));
		m_sPtrStrComp->SetText(m_debugStr);

		SetTextRect(Rect2D<float>(10.0f, 10.0f, 200.0f, 400.0f));
		SetDrawLayer(20);
	}

	void DebugLog::OnUpdate() 
	{
		m_sPtrStrComp->SetText(m_debugStr);
		m_debugStr = m_logTitle;
	}

	void DebugLog::AddDebugStr(const wstring& debugStr)
	{
		m_debugStr += debugStr;
	}

	void DebugLog::SetTextRect(const Rect2D<float>& textRect)
	{
		float rectLeft = textRect.left;
		float rectTop = textRect.top;
		float rectRight = textRect.left + textRect.right;
		float rectBottom = textRect.top + textRect.bottom;
		m_sPtrStrComp->SetTextRect(Rect2D<float>(rectLeft, rectTop, rectRight, rectBottom));
	}
}