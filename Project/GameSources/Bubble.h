#pragma once
#include "stdafx.h"

namespace basecross
{
	enum class BubbleAbility
	{
		ShootBubble,
		RideBubble,
		TranpolineBubble,
	};

	class Bubble : public GameObject
	{
		unordered_map<BubbleAbility,bool> m_abilities;
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticDraw> m_draw;
		shared_ptr<CollisionSphere> m_col;
		weak_ptr<GameObject> m_parent;
		Vec3 m_parentForward;
		Vec3 m_dir;
		Vec3 m_scale;

		float m_speedRatio;
		float m_initialVelocity;
		float m_currentVelocity;
		float m_upwardVelocity;
		bool m_isTimeStart;
		float m_limitTime;
		bool m_isSpawnedTrampoline;
		bool m_isHit;
		float m_moveTime;
		float m_moveTimeLimit;

	public :
		Bubble::Bubble(const shared_ptr<Stage>& stage,const shared_ptr<GameObject>& parent);
		Bubble::~Bubble();

		void OnCreate() override;
		void OnUpdate() override;

		// ----------------------------------
		//　泡の動きの挙動
		// ----------------------------------
		void BubbleMove();
		
		bool HasAblity(BubbleAbility ability)
		{
			auto it = m_abilities.find(ability);

			if (it == m_abilities.end()) return false;
			return it->second;
		}

		void SetAbility(BubbleAbility ability, bool value)
		{
			// キーとそのキーがtureかfalseかをセットする
			m_abilities[ability] = value;
		}

		void BubbleAddAblity(BubbleAbility ability);

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
