#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Bubble::Bubble(const shared_ptr<Stage>& stage, const shared_ptr<GameObject>& parent) :
		GameObject(stage),
		m_parent(parent),
		m_speed(1.0f),
		m_initialVelocity(2.5f),
		m_currentVelocity(m_initialVelocity),
		m_upwardVelocity(0.0f)
	{

	}

	Bubble::~Bubble()
	{

	}

	void Bubble::OnCreate()
	{
		// 親の情報の取得
		auto parentLock = m_parent.lock();
		m_parentForward = parentLock->GetComponent<Transform>()->GetForward();
		auto parentPos = parentLock->GetComponent<Transform>()->GetPosition();
		
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(parentPos + (m_parentForward * 1.5f));
		m_trans->SetScale(Vec3(0.1f));

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(false);

		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"DEFAULT_SPHERE");

		AddTag(L"Bubble");
	}

	void Bubble::OnUpdate()
	{
		BubbleMove();
	}

	void Bubble::BubbleMove()
	{
		auto& app = App::GetApp();
		auto elapsed = app->GetElapsedTime();

		auto pos = m_trans->GetPosition();
		float velocityZero = 0.0;
		float decelerationStartRatio = 0.5f;

		// 現在速度が0になるまで
		if (m_currentVelocity > velocityZero)
		{
			// 減少速度
			m_currentVelocity -= m_speed * elapsed;
			// 速度の割合
			auto speedRatio = m_currentVelocity / m_initialVelocity;
			pos.z += m_currentVelocity * elapsed;

			// 割合が浮遊させる割合になったら
			if (speedRatio < decelerationStartRatio)
			{
				// 開始割合 - 現在の割合 / 開始割合
				m_upwardVelocity = (decelerationStartRatio - speedRatio) / decelerationStartRatio;

				// 上昇速度の最低が0、最大を1
				m_upwardVelocity = clamp(m_upwardVelocity, 0.0f, 1.0f);

				pos.y += m_upwardVelocity * elapsed;
			}
		}

		m_trans->SetPosition(pos);
	}
}
