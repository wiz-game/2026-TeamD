#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	void GameManager::EnterGameMode(ENUM_GameMode gameMode)
	{
		switch (gameMode)
		{
		case ENUM_GameMode::Title:
			App::GetApp()->GetScene<Scene>()->ChangeStage(L"TitleStage");
			break;
		case ENUM_GameMode::Select:
			App::GetApp()->GetScene<Scene>()->ChangeStage(L"SelectStage");
			break;
		case ENUM_GameMode::Play:
			App::GetApp()->GetScene<Scene>()->ChangeStage(m_serectGameStage);
			break;
		case ENUM_GameMode::Menu:
			break;
		case ENUM_GameMode::Editor:
			SetAllGameObjectsUpdateActive(false);
			StageEditor::Instance().StartEditor();
			InputManager::Instance().ResetWheelDelta();
			break;
		default:
			break;
		}
	}

	void GameManager::ExitGameMode(ENUM_GameMode gameMode)
	{
		switch (gameMode)
		{
		case ENUM_GameMode::Title:
			break;
		case ENUM_GameMode::Select:
			break;
		case ENUM_GameMode::Play:
			break;
		case ENUM_GameMode::Menu:
			break;
		case ENUM_GameMode::Editor:
			SetAllGameObjectsUpdateActive(true);
			StageEditor::Instance().EndEditor();
			InputManager::Instance().ResetWheelDelta();
			break;
		default:
			break;
		}
	}

	void GameManager::SetAllGameObjectsUpdateActive(bool isActive)
	{
		for (auto& gameObject : App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetGameObjectVec())
			gameObject->SetUpdateActive(isActive);
	}

	void GameManager::RegisterDebugLog(const wstring& logName, const wstring& debugLog)
	{
		if (!m_isDebug) return;

		if (m_sPtrDebugLog)
		{
			m_sPtrDebugLog->AddDebugStr(logName, debugLog);
		}
		else
		{
			m_sPtrDebugLog = App::GetApp()->GetScene<Scene>()->GetActiveStage()->AddGameObject<DebugLog>();
			m_sPtrDebugLog->SetTextRect(Rect2D<float>(10.0f, 10.0f, 300.0f, 400.0f));
			m_sPtrDebugLog->AddDebugStr(logName, debugLog);
		}
	}

	void GameManager::Initialize(const bool& isDebug)
	{
		SetIsDebug(isDebug);
	}
	
	void GameManager::RemoveDebugLog()
	{
		if (m_sPtrDebugLog)
		{
			m_sPtrDebugLog->DestroyGameObject();
			m_sPtrDebugLog = nullptr;
		}
	}
	
	void GameManager::SetGameMode(ENUM_GameMode gameMode)
	{
		ExitGameMode(m_gameMode);
		m_gameMode = gameMode;
		EnterGameMode(m_gameMode);
	}

	void GameManager::SetGameModeAfterTransition(ENUM_GameMode gameMode)
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		//m_sPtrUISlide = stage->AddGameObject<UISlide>();
		//m_transitionAfterGameMode = gameMode;
	}

	void GameManager::ChangeSelectGameStage(const int& incrDecrNum)
	{
		// 現在のステージ番号を取得
		int stageNum = _wtoi(m_serectGameStage.substr(m_serectGameStage.find_last_of(L"_") + 1).c_str());

		stageNum += incrDecrNum;
		stageNum = max(stageNum, GAMESTAGE_MIN);
		stageNum = min(stageNum, GAMESTAGE_MAX);

		m_serectGameStage = L"GameStage_" + to_wstring(stageNum);
	}
}