#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	void GameManager::EnterGameMode(ENUM_GameMode gameMode)
	{
		switch (gameMode)
		{	
		case ENUM_GameMode::Play:
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

	void GameManager::Initialize()
	{
		SetIsDebug(true);
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
}