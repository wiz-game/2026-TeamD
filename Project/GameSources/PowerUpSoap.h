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
	public:
		// 構築と破棄
		PowerUpSoap(const shared_ptr<Stage>& stage) :
			ItemBase(stage)
		{}
		virtual ~PowerUpSoap()
		{}

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		//virtual void OnDraw() override; // 描画

		void GetSoapOfCoolDown();
	};

}
//end basecross
