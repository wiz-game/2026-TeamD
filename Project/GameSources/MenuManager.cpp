#include "stdafx.h"
#include "Project.h"
#include "MenuManager.h"

namespace basecross
{
	// インスタンス
	MenuManager& MenuManager::Instance()
	{
		static MenuManager instance;
		return instance;
	}

	// ポーズメニューを開く
	void MenuManager::Pause()
	{
		SetAllUpdateActive(false);
		SetMenuMode(ENUM_MenuMode::MenuStart);
		ChangeMenuMode();
	}

	// ポーズメニューを閉じる
	void MenuManager::ClosePause()
	{
		SetAllUpdateActive(true);
		SetMenuMode(ENUM_MenuMode::Default);
		ChangeMenuMode();
	}


	// ステージの全オブジェクトのUpdateを管理する
	void MenuManager::SetAllUpdateActive(const bool& isUpdateActive)
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto gameObjects = stage->GetGameObjectVec();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetUpdateActive(isUpdateActive);
		}
	}

	// メニュー画面の表示非表示を切り替える関数
	void MenuManager::UIDrawActive(bool isActive, vector<shared_ptr<UIBase>> uipointers)
	{
		for (auto& uipointer : uipointers)
		{
			uipointer->GetComponent<PCTSpriteDraw>()->SetDrawActive(isActive);
		}
	}

	// 選択肢のパラメータを設定する関数
	void MenuManager::SetUIDiffuse(int i, const vector<shared_ptr<UIBase>>& uipointers, const Col4& diffuse)
	{
		if (i < 0 || i >= uipointers.size())return;

		auto ui = uipointers[i];
		if (!ui)return;

		auto draw = ui->GetComponent<PCTSpriteDraw>();
		if (ui)
		{
			draw->SetDiffuse(diffuse);
		}
	}

	void MenuManager::SetUISize(int i, const vector<shared_ptr<UIBase>>& uipointers, const float& scale)
	{
		if (i < 0 || i >= uipointers.size()) return;

		auto ui = uipointers[i];
		if (!ui) return;

		ui->SetImageSizeRatio(scale);
	}

	void MenuManager::ChangeUISize(float size)
	{
		switch (GetMenuMode())
		{

			// メニュー画面の時
		case ENUM_MenuMode::MenuStart:
			switch (GetMenuUI())
			{
			case ENUM_MenuStart::Restart:
				SetUISize(0, m_uidefaults, size);
				break;

			case ENUM_MenuStart::Setting:
				SetUISize(1, m_uidefaults, size);
				break;

			case ENUM_MenuStart::Howtoplay:
				SetUISize(2, m_uidefaults, size);
				break;

			case ENUM_MenuStart::Retitle:
				SetUISize(3, m_uidefaults, size);
				break;
			}
			break;

			// 設定画面の時
		case ENUM_MenuMode::Setting:
			break;

			// あそびかた画面の時
		case ENUM_MenuMode::Howtoplay:
			break;
		}
	}

	// メニュー画面の選択肢のパラメータを変える関数
	void MenuManager::ChangeUIParam()
	{
		// ディフューズ
		Col4 defaultCol = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		Col4 elseCol = Col4(0.7f, 0.7f, 0.7f, 1.0f);

		// エミッシブ
		Col4 defaultEmi = Col4(0.0f);
		Col4 elseEmi = Col4(0.0f, 0.0f, 0.0f, 0.0f);

		// スケール
		float defaultScale = 0.25f;
		float elseScale = 0.20f;

		switch (GetMenuMode())
		{

			// メニュー画面の時
		case ENUM_MenuMode::MenuStart:
			switch (GetMenuUI())
			{
			case ENUM_MenuStart::Restart:
				SetUIDiffuse(0, m_uidefaults, defaultCol);
				SetUIDiffuse(1, m_uidefaults, elseCol);
				SetUIDiffuse(2, m_uidefaults, elseCol);
				SetUIDiffuse(3, m_uidefaults, elseCol);
				break;
			case ENUM_MenuStart::Setting:
				SetUIDiffuse(0, m_uidefaults, elseCol);
				SetUIDiffuse(1, m_uidefaults, defaultCol);
				SetUIDiffuse(2, m_uidefaults, elseCol);
				SetUIDiffuse(3, m_uidefaults, elseCol);
				break;
			case ENUM_MenuStart::Howtoplay:
				SetUIDiffuse(0, m_uidefaults, elseCol);
				SetUIDiffuse(1, m_uidefaults, elseCol);
				SetUIDiffuse(2, m_uidefaults, defaultCol);
				SetUIDiffuse(3, m_uidefaults, elseCol);
				break;
			case ENUM_MenuStart::Retitle:
				SetUIDiffuse(0, m_uidefaults, elseCol);
				SetUIDiffuse(1, m_uidefaults, elseCol);
				SetUIDiffuse(2, m_uidefaults, elseCol);
				SetUIDiffuse(3, m_uidefaults, defaultCol);
				break;
			}
			break;

			// 設定画面の時
		case ENUM_MenuMode::Setting:
			break;

			// あそびかた画面の時
		case ENUM_MenuMode::Howtoplay:
			break;
		}

	}

	// メニュー画面の切り替え
	void MenuManager::ChangeMenuMode()
	{
		switch (GetMenuMode())
		{
			// メニューを閉じてるとき
		case ENUM_MenuMode::Default:
			UIDrawActive(false, m_uidefaults);
			UIDrawActive(false, m_uiframes);
			UIDrawActive(false, m_uisettings);
			UIDrawActive(false, m_uihowtoplays);
			break;

			// メニューを開いたとき
		case ENUM_MenuMode::MenuStart:
			UIDrawActive(true, m_uiframes);
			UIDrawActive(true, m_uidefaults);
			UIDrawActive(false, m_uisettings);
			UIDrawActive(false, m_uihowtoplays);
			break;

			// 設定画面
		case ENUM_MenuMode::Setting:
			UIDrawActive(true, m_uiframes);
			UIDrawActive(false, m_uidefaults);
			UIDrawActive(true, m_uisettings);
			UIDrawActive(false, m_uihowtoplays);
			break;

			// あそびかた画面
		case ENUM_MenuMode::Howtoplay:
			UIDrawActive(true, m_uiframes);
			UIDrawActive(false, m_uidefaults);
			UIDrawActive(false, m_uisettings);
			UIDrawActive(true, m_uihowtoplays);
			break;
		}
	}

	// 上下入力時にメニューを切り替える関数
	void MenuManager::ChangeSelectMenuMode(const int& num)
	{
		auto menuNow = GetMenuUI();
		int menuAfter = static_cast<int>(menuNow);
		menuAfter += num;

		if (menuAfter == -1) menuAfter = 3;
		if (menuAfter == 4) menuAfter = 0;

		ENUM_MenuStart setAfter = static_cast<ENUM_MenuStart>(menuAfter);

		SetMenuUI(setAfter);
	}

//----セッター関数群-------------------------------------------------------------------

	void MenuManager::SetUIFrames(const vector<shared_ptr<UIBase>>& uiframes)
	{
		m_uiframes = uiframes;
	}
	void MenuManager::SetUIDefaults(const vector<shared_ptr<UIBase>>& uidefaults)
	{
		m_uidefaults = uidefaults;
	}
	void MenuManager::SetUISettings(const vector<shared_ptr<UIBase>>& uisettings)
	{
		m_uisettings = uisettings;
	}
	void MenuManager::SetUIHowtoplays(const vector<shared_ptr<UIBase>>& uihowtoplays)
	{
		m_uihowtoplays = uihowtoplays;
	}
	void MenuManager::SetUIDiffuse(const Col4& uidiffuse)
	{
		m_uidiffuse = uidiffuse;
	}
	void MenuManager::SetUIScale(const float& uiscale)
	{
		m_uiscale = uiscale;
	}
	void MenuManager::SetMenuMode(ENUM_MenuMode menumode)
	{
		m_menuMode = menumode;
	}
	void basecross::MenuManager::SetMenuUI(ENUM_MenuStart menuui)
	{
		m_menuUI = menuui;
	}
//--------------------------------------------------------------------------------------
}