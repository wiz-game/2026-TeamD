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
		Vec3 m_dir;

		float m_speed;
		float m_speedRatio;
		float m_initialVelocity;
		float m_currentVelocity;
		float m_upwardVelocity;
		bool m_isTimeStart;
		float m_limitTime;
		bool m_isSpawnedTrampoline;
		bool m_isHit;

	public :
		Bubble::Bubble(const shared_ptr<Stage>& stage,const shared_ptr<GameObject>& parent);
		Bubble::~Bubble();

		void OnCreate() override;
		void OnUpdate() override;

		// ----------------------------------
		//　泡の動きの挙動
		// ----------------------------------
		void BubbleMove();
		
		Vec3 GetCameraForward();

		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other);

	};

	class ViewBubble : public GameObject
	{
		weak_ptr<GameObject> m_parent;
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticInstanceDraw> m_draw;
		const vector<Vec3> *m_vertices;

	public:
		ViewBubble::ViewBubble(const shared_ptr<Stage>& stage, const vector<Vec3>* vertices);
		ViewBubble::~ViewBubble();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void CreateInstance();
	};
}
