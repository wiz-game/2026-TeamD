/*!
@file Dirt.h
@brief 汚れ
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class Dirt : public GameObject
	{
	private:
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticDraw> m_draw;
		int m_nowDeleteCount;
		float m_alphaColor;
	public:
		Dirt
		(
			const shared_ptr<Stage>& StagePtr,
			const STRUCT_ObjectParam& objectParam
		);
		~Dirt();

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other);

	};
}