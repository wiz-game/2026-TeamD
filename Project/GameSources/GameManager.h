/*!
@file GameManager.h
@brief ゲームマネージャー 
@copyright 菅野 皐
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	enum class ENUM_GameMode
	{
		Play,
		Menu,
		Editor
	};

	class GameManager
	{
	private:
		shared_ptr<DebugLog> m_sPtrDebugLog = nullptr;

		ENUM_GameMode m_gameMode = ENUM_GameMode::Play;

		bool m_isDebug = false;
	private:
		GameManager() {}
		virtual ~GameManager() {}
		
		void EnterGameMode(ENUM_GameMode gameMode);
		void ExitGameMode(ENUM_GameMode gameMode);

		// 全てのゲームオブジェクトの更新フラグを変更する
		void SetAllGameObjectsUpdateActive(bool isActive);
		void RegisterDebugLog(const wstring& logName, const wstring& debugLog);
	public:
		static GameManager& Instance()
		{
			static GameManager instance;
			return instance;
		}

		void Initialize(const bool& isDebug = false);
		
		void AddDebugStr(const wstring& logName, const wstring& debugLog) { RegisterDebugLog(logName, debugLog); }
		void AddDebugStr(const wstring& logName, const int& debugLog)     { RegisterDebugLog(logName, to_wstring(debugLog)); }
		void AddDebugStr(const wstring& logName, const float& debugLog)   { RegisterDebugLog(logName, to_wstring(debugLog)); }

		void RemoveDebugLog();
		
		// アクセサー
		ENUM_GameMode GetGameMode() const { return m_gameMode; }
		void SetGameMode(ENUM_GameMode gameMode);

		bool GetIsDebug() const { return m_isDebug; }
		void SetIsDebug(bool isDebug) { m_isDebug = isDebug; }
	};

}
