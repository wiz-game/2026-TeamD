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
	
	public:
		Dirt
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		~Dirt();

		virtual void OnCreate()override;
		virtual void OnDestroy()override;
    
		void SetDirtHP(const float HP);
		float GetDirtHP() const { return m_HP; }

		void SetDirtState(DirtCondition state);
		void EnterDirtState(DirtCondition state);
		void ExitDirtState(DirtCondition state);
	};
}