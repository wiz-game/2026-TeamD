#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Bubble::Bubble(const shared_ptr<Stage>& stage, const shared_ptr<GameObject>& parent) :
		GameObject(stage),
		m_parent(parent),
		m_speed(1.25f),
		m_speedRatio(0.0f),
		m_initialVelocity(2.5f),
		m_currentVelocity(m_initialVelocity),
		m_upwardVelocity(0.0f),
		m_isTimeStart(false),
		m_limitTime(2.0f)
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
		m_trans->SetPosition(parentPos + (m_parentForward * 1.25f));
		m_trans->SetScale(Vec3(0.75f));

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
			m_speedRatio = m_currentVelocity / m_initialVelocity;
			pos.z += m_currentVelocity * elapsed;
		}

		// 現在の速度の割合が半分になったら上昇を始める
		if (m_speedRatio < decelerationStartRatio)
		{
			m_isTimeStart = true;

			// 0.5 -> 0.0fに向かうほど t: 0 -> 1
			auto t = (decelerationStartRatio - m_speedRatio) / decelerationStartRatio;
			t = clamp(t,0.0f, 1.0f);

			// 1から2までの補間係数
			m_upwardVelocity = 1.0f + (2.0f - 1.0f) * t;

			pos.y += m_upwardVelocity * elapsed;
		}

		if (m_isTimeStart)
		{
			m_limitTime -= 1.0f * elapsed;
		}

		if (m_limitTime < 0.0f)
		{
			GetStage()->RemoveGameObject<Bubble>(GetThis<Bubble>());
			return;
		}

		m_trans->SetPosition(pos);
	}

	void Bubble::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (!Other->FindTag(L"Ground"))return;

		auto ground = dynamic_pointer_cast<Ground>(Other);
		
		if (ground)
		{
			GetStage()->AddGameObject<TrampolineBubbles>(m_trans->GetPosition());
		}
	}

}
