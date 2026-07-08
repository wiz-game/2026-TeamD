#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	void GameManager::EnterGameMode(ENUM_GameMode gameMode)
	{
		SoundManager::Instance().StopBGM();
		SoundManager::Instance().AllStopSE();

		auto scene = App::GetApp()->GetScene<Scene>();
		switch (gameMode)
		{
		case ENUM_GameMode::Title:
			scene->ChangeStage(L"TitleStage");
			SoundManager::Instance().PlayBGM(L"Title_BGM");
			break;
		case ENUM_GameMode::Select:
			scene->ChangeStage(L"SelectStage");
			SoundManager::Instance().PlayBGM(L"Title_BGM");
			break;
		case ENUM_GameMode::GameClear:
			scene->ChangeStage(L"GameClearStage");
			SoundManager::Instance().PlaySE(L"GameClear_BGM");
			break;
		case ENUM_GameMode::GameOver:
			scene->ChangeStage(L"GameOverStage");
			SoundManager::Instance().PlaySE(L"GameOver_BGM");
			break;
		case ENUM_GameMode::Play:
			if (scene->GetNowStageName() != m_selectGameStage)
				scene->ChangeStage(m_selectGameStage);
			break;
		case ENUM_GameMode::Menu:
			MenuManager::Instance().Pause();
			MenuManager::Instance().SetMenuUI(ENUM_MenuStart::Restart);
			break;
		case ENUM_GameMode::Editor:
			SetAllGameObjectsUpdateActive(false);
			StageEditor::Instance().StartEditor();
			InputManager::Instance().ResetWheelDelta();
			break;
		case ENUM_GameMode::PlayMovie:
			MovieManager::Instance().SetMovieType(MovieType::Play);
			break;
		case ENUM_GameMode::GameOverMovie:
			MovieManager::Instance().SetMovieType(MovieType::GameOver);
			break;
		case ENUM_GameMode::GameClearMovie:
			MovieManager::Instance().SetMovieType(MovieType::GameClear);
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
		case ENUM_GameMode::GameClear:
			break;
		case ENUM_GameMode::GameOver:
			break;
		case ENUM_GameMode::Play:
			break;
		case ENUM_GameMode::Menu:
			MenuManager::Instance().ClosePause();
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
		{
			gameObject->SetUpdateActive(isActive);

			if (gameObject->FindTag(L"CollisionObject"))
			{
				auto draw = gameObject->GetComponent<PNTStaticDraw>(false);
				if (draw) draw->SetDrawActive(!isActive);
			}
		}
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
		if (m_transitionAfterGameMode != ENUM_GameMode::Null) return;

		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		stage->AddGameObject<UISlide>(STRUCT_UIParam(L"Awa", Vec3(10.0f, -1400.0f, 0.0f), 0.3f), 400.0f);
		stage->AddGameObject<UISlide>(STRUCT_UIParam(L"Awa", Vec3(200.0f, -1200.0f, 0.0f), 0.3f), 400.0f);
		stage->AddGameObject<UISlide>(STRUCT_UIParam(L"Awa", Vec3(-630.0f, -1100.0f, 0.0f), 0.3f), 400.0f);
		stage->AddGameObject<UISlide>(STRUCT_UIParam(L"Awa", Vec3(490.0f, -1400.0f, 0.0f), 0.3f), 400.0f);
		stage->AddGameObject<UISlide>(STRUCT_UIParam(L"Awa", Vec3(-320.0f, -1000.0f, 0.0f), 0.3f), 400.0f);
		stage->AddGameObject<UISlide>(STRUCT_UIParam(L"Awa", Vec3(-120.0f, -1600.0f, 0.0f), 0.3f), 400.0f);
		stage->AddGameObject<UISlide>(STRUCT_UIParam(L"Awa", Vec3(490.0f, -900.0f, 0.0f), 0.3f), 400.0f);
		stage->AddGameObject<UITransitionSlide>(STRUCT_UIParam(L"Awas", Vec3(0.0f, -3000.0f, 0.0f), 1.3f), 600.0f, true);

		m_transitionAfterGameMode = gameMode;
	}

	void GameManager::ChangeSelectGameStage(const int& incrDecrNum)
	{
		// 現在のステージ番号を取得
		int stageNum = _wtoi(m_selectGameStage.substr(m_selectGameStage.find_last_of(L"_") + 1).c_str());

		stageNum += incrDecrNum;
		stageNum = max(stageNum, GAMESTAGE_MIN);
		stageNum = min(stageNum, GAMESTAGE_MAX);

		m_selectGameStage = L"GameStage_" + to_wstring(stageNum);
	}
}