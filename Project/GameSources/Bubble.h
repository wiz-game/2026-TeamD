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
		static unordered_map<BubbleAbility,bool> m_unlockedAvilities;
		BubbleAbility currentAbility;
		shared_ptr<Transform> m_trans;
		shared_ptr<PNTStaticDraw> m_draw;
		shared_ptr<PNTStaticInstanceDraw> m_activeDraw;
		shared_ptr<CollisionSphere> m_col;
		weak_ptr<GameObject> m_parent;
		Vec3 m_parentForward;
		Vec3 m_scale;
		Vec3 m_pos;
		Vec3 m_forward;
		Vec3 m_moveDir;

		// 泡のスピード
		float m_speed;
		// 泡のスピードの倍率
		float m_speedRatio;
		// 初速度
		float m_initialVelocity;
		// 現在の速度
		float m_currentVelocity;
		// 上昇速度
		float m_upwardVelocity;
		// 生存時間のカウント初め
		bool m_isTimeStart;
		// 生存時間
		float m_limitTime;
		// トランポリン泡が生成されているか
		bool m_isSpawnedTrampoline;
		// 泡が当たったかどうか
		bool m_isHit;
		// 泡が動いているかどうか
		float m_moveTime;
		// 泡が動いている時間の制限
		float m_moveTimeLimit;
		// 泡が発射されたかどうか
		bool m_isShoot;
		// 泡が動いているかどうか
		bool m_isBubbleMove;

		// 相殺力
		float m_HP;

		bool m_isTranpolineBubble;
		bool m_isRideBubble;

	public :
		Bubble::Bubble(const shared_ptr<Stage>& stage,const shared_ptr<GameObject>& parent,const Vec3& scale,const float& initialVelocity,const float& HP);
		Bubble::~Bubble();

		void OnCreate() override;
		void OnUpdate() override;

		void ShootBubble();

		//　泡の動きの挙動
		void BubbleMove();
		
		void UnlockAbility(BubbleAbility ability)
		{
			m_unlockedAvilities[ability] = true;
		}

		void ApplyAblity(BubbleAbility ability);

		bool CanUseAbility(BubbleAbility a)
		{
			auto it = m_unlockedAvilities.find(a);

			if (it == m_unlockedAvilities.end())
				return false;

			return it->second;
		}

		// 泡のアビリティを追加する
		void BubbleAddAblity(BubbleAbility ability);

		// カメラの前方向を得る
		Vec3 GetCameraForward();

		// 相殺力をセットする
		void SetBubbleHP(float force)
		{
			m_HP = force;
		}

		// 相殺力を得る
		float GetBubbleHP()
		{
			return m_HP;
		}

		// 相殺力を得る
		void DecreaseBubbleHP(float force)
		{
			m_HP -= force;
		}

		void ResolveCounteract(Bubble& bubble, Dirt& dirt);

		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other);

		void CreateActiveInstances();
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
