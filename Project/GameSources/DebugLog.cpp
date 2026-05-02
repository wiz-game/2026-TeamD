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
		m_debugStrs.insert({ L"LogTitle", logTitle });
	}

	void DebugLog::OnCreate() 
	{
		m_sPtrStrComp = AddComponent<StringSprite>();

		m_sPtrStrComp->SetBackColor(Col4(0.0f, 0.0f, 0.0f, 0.8f));
		m_sPtrStrComp->SetFontColor(Col4(0.0f, 1.0f, 0.0f, 1.0f));
		m_sPtrStrComp->SetText(m_debugStrs[L"LogTitle"]);

		SetTextRect(Rect2D<float>(10.0f, 10.0f, 200.0f, 400.0f));
		SetDrawLayer(20);
	}

	void DebugLog::AddDebugStr(const wstring& logName, const wstring& debugLog)
	{
		// すでに同じlogNameが存在する場合は更新、存在しない場合は追加
		if (m_debugStrs.find(logName) != m_debugStrs.end())
		{
			m_debugStrs[logName] = debugLog;
		}
		else
		{
			m_debugStrs.insert({ logName, debugLog });
		}

		UpdateDebugLog();
	}

	void DebugLog::UpdateDebugLog()
	{
		// StringSpriteに渡す変数を定義、タイトルはsecondのみ描画
		wstring logStr = m_debugStrs[L"LogTitle"] + L"\n";

		map<wstring, wstring>::iterator it;
		for (it = m_debugStrs.begin(); it != m_debugStrs.end(); it++)
		{
			// タイトル以外を全て描画
			if (it->first != L"LogTitle")
			{
				logStr += it->first + L" : ";
				logStr += it->second + L"\n";
			}
		}

		m_sPtrStrComp->SetText(logStr);
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