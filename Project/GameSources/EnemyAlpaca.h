/*!
@file EnemyAlpaca.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "EnemyBase.h"

namespace basecross 
{


	//--------------------------------------------------------------------------------------
	//	class GenericSprite : public EnemyBase;
	//--------------------------------------------------------------------------------------
	class EnemyAlpaca : public EnemyBase
	{
	private:
		//EnemyBase* m_eBase;
		Vec3 m_Position;
		Vec3 m_Scale;
		Vec3 m_Rotation;

		float m_HP;
		float m_Speed;
	public:
		// 構築と破棄
		EnemyAlpaca(const shared_ptr<Stage>& stage,float hp) :
			EnemyBase(stage),
			m_Position(0.0f, 61.0f, 0.0f),
			m_Scale(0.3f, 0.3f, 0.3f),
			m_Rotation(0.0f,0.0f,0.0f),
			m_HP(10),
			m_Speed(1.0f)
		{
		}
		virtual ~EnemyAlpaca()
		{
		}

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		//virtual void OnDraw() override; // 描画
	};

}
//end basecross
