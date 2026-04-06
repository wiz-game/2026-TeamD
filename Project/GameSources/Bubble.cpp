#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Bubble::Bubble(const shared_ptr<Stage>& stage, const shared_ptr<GameObject>& parent) :
		GameObject(stage),
		m_parent(parent)
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

	}

}
