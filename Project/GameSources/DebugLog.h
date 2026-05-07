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
		map<wstring, wstring> m_debugStrs = {};
		shared_ptr<StringSprite> m_sPtrStrComp = nullptr;
	private:
	public:
		DebugLog(const shared_ptr<Stage>& stagePtr, const wstring& logTitle = L"-DebugLog-");
		virtual ~DebugLog() {}

		virtual void OnCreate() override;
		virtual void OnDestroy() override { GetStage()->RemoveGameObject<DebugLog>(GetThis<DebugLog>()); }

		void AddDebugStr(const wstring& logName, const wstring& debugLog);
		void UpdateDebugLog();

		void SetTextRect(const Rect2D<float>& textRect);
	};
}