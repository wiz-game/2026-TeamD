/*!
@file Move.h
@brief ムーブコンポーネント
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	class Move : public Component;
	//--------------------------------------------------------------------------------------
	class Move : public Component
	{
	private:
		float m_Speed;
	public:
		// 構築と破棄
		Move(const shared_ptr<GameObject>& gameObject);
		virtual ~Move()
		{
		}

		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		virtual void OnDraw() override; // 描画
		void VectorMove(Vec3 Input);
	};

}
//end basecross
