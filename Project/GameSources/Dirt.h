/*!
@file Dirt.h
@brief 汚れ
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	enum class DirtCondition
	{
		DirtMax,
		DirtHalf,
		DirtClean
	};

	class Dirt : public GameObject
	{
	private:
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticDraw> m_draw;
		DirtCondition m_dirtCondition;
		int m_nowDeleteCount;
		float m_alphaColor;
		// 相殺力
		float m_HP;
		// エフェクトが再生されたか
		bool m_effectPlyaerd;
	public:
		Dirt
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		~Dirt();

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnUpdate2()override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other);

		void DecreaseDirtHP(float force)
		{
			m_HP -= force;
		}

		void SetDirtHP(float HP)
		{
			m_HP = HP;
		}

		float GetDirtHP()
		{
			return m_HP;
		}

		void SetDirtState(DirtCondition state);
		void EnterDirtState(DirtCondition state);
		void ExitDirtState(DirtCondition state);
	};
}