/*!
@file ItemBase.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	class GenericSprite : public GameObject;
	//--------------------------------------------------------------------------------------
	class ItemBase : public GameObject
	{
	private:
	protected:

	public:
		// 構築と破棄
		ItemBase(const shared_ptr<Stage>& stage) :
			GameObject(stage)
		{}
		virtual ~ItemBase()
		{}

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		//virtual void OnDraw() override; // 描画

		void GetItem(const shared_ptr<GameObject>& gameObject);
		void GetSoapOfCoolDown();

		// --- 当たり判定 ---
		void OnCollisionEnter(shared_ptr<GameObject>& Other);	//	当たり判定
		void OnCollisionExecute(shared_ptr<GameObject>& Other);
		void OnCollisionExit(shared_ptr<GameObject>& Other);
		// ------------------
	};

}
//end basecross
