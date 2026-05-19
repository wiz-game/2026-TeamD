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
		JoltManager m_jphManger;
		Vec3 m_collManagerPos = Vec3(0.0f);
		Vec3 m_collisionRange = Vec3(100.0f, 1000.0f, 100.0f);
		string m_stageNum;

		void CreateViewLight();
		void CreatePlayer();

		void SetCollRange();
	public:
		GameStage(const wstring& stageNum);
		virtual ~GameStage() {}
		
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;
	};
}