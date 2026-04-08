#pragma once
#include "stdafx.h"

namespace basecross
{
	class Bubble : public GameObject
	{
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticDraw> m_draw;
		weak_ptr<GameObject> m_parent;
		Vec3 m_parentForward;

		float m_speed;
		float m_speedRatio;
		float m_initialVelocity;
		float m_currentVelocity;
		float m_upwardVelocity;
		bool m_isTimeStart;
		float m_limitTime;

	public :
		Bubble::Bubble(const shared_ptr<Stage>& stage,const shared_ptr<GameObject>& parent);
		Bubble::~Bubble();

		void OnCreate() override;
		void OnUpdate() override;

		// ----------------------------------
		//　泡の動きの挙動
		// ----------------------------------
		void BubbleMove();

	};
}
