/*!
@file PowerUpSoap.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	class GenericSprite : public GameObject;
	//--------------------------------------------------------------------------------------
	class PowerUpSoap : public ItemBase
	{
	private:
		std::shared_ptr<Transform> m_transform;
		std::shared_ptr<PNTStaticDraw> m_draw;
		std::shared_ptr <CollisionObb> m_colObb;
		std::shared_ptr<Gravity> m_gravity;

		Vec3 m_Position;
		Vec3 m_Scale;
		Vec3 m_Rotation;

		float m_RotationSpeed;
	public:
		// 構築と破棄
		PowerUpSoap(const shared_ptr<Stage>& stage) :
			ItemBase(stage),
			m_Scale(1.1f, 0.5f, 1.1f),
			m_RotationSpeed(45.0f)
		{}
		virtual ~PowerUpSoap()
		{}

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		//virtual void OnDraw() override; // 描画

		void Rotation();
		void UpDown();
		void DebugStr();
	};

}
//end basecross
