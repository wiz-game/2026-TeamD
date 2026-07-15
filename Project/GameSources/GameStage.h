/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"
#include "JoltManager.h"

namespace basecross
{
	class GameStage : public Stage
	{
	private:
		Vec3 m_collManagerPos = Vec3(0.0f);
		Vec3 m_collisionRange = Vec3(3000.0f, 1.0f, 3000.0f);
		string m_stageNum;
		
		bool m_isGameClear;
		shared_ptr<UITransitionSlide> uiAwasSlide;

		// メニュー画面のUI
		vector<shared_ptr<UIBase>> m_uiframes;
		vector<shared_ptr<UIBase>> m_uidefaults;
		vector<shared_ptr<UIBase>> m_uisettings;
		vector<shared_ptr<UIBase>> m_uihowtoplays;

		// メニュー画面のパラメータ
		Col4 m_uiDiffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		float m_selectScale = 0.25f;

		// ゲーム画面のUI
		vector<shared_ptr<UIStatueNum>> m_uistatues;

		void CreateViewLight();
		void CreatePlayer();
		void CreateMenu();
		void CreateGameUI();

		void SetCollRange();
	public:
		GameStage(const wstring& stageNum);


		virtual ~GameStage() {}
		
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}