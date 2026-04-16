/*!
@file DebugLog.h
@brief デバッグ用文字列を表示、管理する
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	class DebugLog : public GameObject
	{
	private:
		wstring m_logTitle = L"";
		wstring m_debugStr = m_logTitle;
		shared_ptr<StringSprite> m_sPtrStrComp;
	private:
	public:
		DebugLog(const shared_ptr<Stage>& stagePtr, const wstring& logTitle = L"-DebugLog-\n");
		virtual ~DebugLog() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override { GetStage()->RemoveGameObject<DebugLog>(GetThis<DebugLog>()); }

		void AddDebugStr(const wstring& debugStr);

		void SetTextRect(const Rect2D<float>& textRect);
	};
}