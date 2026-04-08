#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TrampolineBubbles::TrampolineBubbles(const shared_ptr<Stage>& stage) :
		GameObject(stage),
		m_isTrampolineActive(false),
		m_bubbleCount(0)
	{

	}

	TrampolineBubbles::~TrampolineBubbles()
	{

	}

	void TrampolineBubbles::OnCreate()
	{
		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"DEFAULT_SPHERE");
		m_draw->SetDrawActive(false);

		m_col = AddComponent<CollisionSphere>();
		m_col->SetDrawActive(true);
		//m_col->SetFixed(true);
		//m_col->SetAfterCollision(AfterCollision::None);

		AddTag(L"TrampolineBubbles");
	}

	void TrampolineBubbles::OnUpdate()
	{
		if (m_bubbleCount >= 5)
		{
			m_draw->SetDrawActive(true);
			m_isTrampolineActive = true;
		}

		wstringstream wss;
		
		wss << m_bubbleCount << endl;

		auto scene = App::GetApp()->GetScene<Scene>();
		scene->SetDebugString(wss.str());
	}

	void TrampolineBubbles::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if(!Other || !Other->FindTag(L"Bubble")) return;

		if (!m_isTrampolineActive)
		{
			m_bubbleCount++;
		}

		auto bubble = dynamic_pointer_cast<Bubble>(Other);
		if (bubble)
		{
			GetStage()->RemoveGameObject<Bubble>(bubble);
		}
	}

}
