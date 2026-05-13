/*!
@file EnemyAlpaca.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{


	//--------------------------------------------------------------------------------------
	//	class GenericSprite : public GameObject;
	//--------------------------------------------------------------------------------------
	class EnemyAlpaca : public GameObject
	{
	public:
		// 構築と破棄
		EnemyAlpaca(const shared_ptr<Stage>& stage) :
			GameObject(stage)
		{
		}
		virtual ~EnemyAlpaca()
		{
		}

		virtual void OnCreate() override; // 初期化
		//virtual void OnUpdate() override; // 更新
		//virtual void OnDraw() override; // 描画
	};

}
//end basecross
