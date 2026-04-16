#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	FurBubble::FurBubble(const shared_ptr<GameObject>& gameObject, const shared_ptr<Stage>& stage) :
		Component(gameObject),
		m_stage(stage)
	{
	}

	FurBubble::~FurBubble()
	{
	}

	void FurBubble::OnCreate()
	{
		m_trans = GetGameObject()->GetComponent<Transform>();

		m_draw = GetGameObject()->GetComponent<PNTStaticDraw>();
		m_vertices = m_draw->GetMeshResource()->GetNumVertices();
		m_draw->GetStaticMeshWorldPositions(m_meshPosition);
		CreateBubble();
	}

	void FurBubble::OnUpdate()
	{
	}

	void FurBubble::OnDraw()
	{

	}

	void FurBubble::CreateBubble()
	{
		m_stage->AddGameObject<ViewBubble>(m_draw, m_vertices, GetGameObject());
	}

}