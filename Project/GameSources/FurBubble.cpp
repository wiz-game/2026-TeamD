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
		m_meshPosition = new vector<Vec3>();

		m_trans = GetGameObject()->GetComponent<Transform>();

		m_draw = GetGameObject()->GetComponent<PNTStaticDraw>();
		GetStaticMeshWorldPositions(m_meshPosition);
		CreateBubble();

	}

	void FurBubble::OnUpdate()
	{
		GetStaticMeshWorldPositions(m_meshPosition);
	}

	void FurBubble::OnDraw()
	{

	}

	void FurBubble::CreateBubble()
	{
		m_stage->AddGameObject<ViewBubble>(m_meshPosition, GetGameObject());
	}

	void FurBubble::GetStaticMeshWorldPositions(vector<Vec3> *vertices)
	{
		if (vertices == nullptr)
		{
			return;
		}

 		m_draw->GetStaticMeshLocalPositions(*vertices);

		// ワールド行列の変換
		auto worldMat = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();

		for (auto& v : *vertices)
		{
			v *= worldMat;
		}
	}

}