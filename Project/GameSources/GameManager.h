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
		Title,
		Select,
		GameClear,
		GameOver,
		Play,
		Menu,
		Editor,
		Movie,
		Null
	};

	class GameManager
	{
	private:
		shared_ptr<DebugLog> m_sPtrDebugLog = nullptr;
		int m_dirtNum;

		ENUM_GameMode m_gameMode = ENUM_GameMode::Title;
		ENUM_GameMode m_transitionAfterGameMode = ENUM_GameMode::Null;

		bool m_isDebug = false;
		wstring m_selectGameStage = L"GameStage_1";

		const int GAMESTAGE_MIN = 1;
		const int GAMESTAGE_MAX = 5;
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
		void SetGameModeAfterTransition(ENUM_GameMode gameMode);

		bool GetIsDebug() const { return m_isDebug; }
		void SetIsDebug(bool isDebug) { m_isDebug = isDebug; }

		wstring GetSelectGameStage() const { return m_selectGameStage; }
		void SetSelectGameStage(const wstring& stageName) { m_selectGameStage = stageName; }
		void ChangeSelectGameStage(const int& incrDecrNum);

		ENUM_GameMode GetTransitionAfterGameMode() const { return m_transitionAfterGameMode; }
		void ResetTransitionAfterGameMode() { m_transitionAfterGameMode = ENUM_GameMode::Null; }

		void AddDirt() { m_dirtNum++; }
		void SubDirt() { m_dirtNum--; }
		void SetDirtNum(const int& dirtNum) { m_dirtNum = dirtNum; }
		int GetDirtNum() const { return m_dirtNum; }
	};
}
