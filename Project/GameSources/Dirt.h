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
		Vec3 m_scale;
		Quat m_quatition;
		Vec3 m_position;
		int m_nowDeleteCount;
		float m_alphaColor;

	public:
		Dirt(
			const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Quatition,
			const Vec3& Position
			);
		~Dirt();

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other);

	};
}