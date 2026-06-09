/*!
@file Ground.cpp
@brief 地面
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Ground::Ground
	(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam
	) :
		GameObject(StagePtr, objectParam)
	{}

	Ground::~Ground()
	{
	}

	void Ground::OnCreate()
	{
		AddTag(L"Ground");
		SetIsEditorSave(true);
		SetAlphaActive(true);

		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());

		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, m_vertices, indices);
		SetVertices();

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->CreateOriginalMesh(m_vertices, indices);
		ptrDraw->SetOriginalMeshUse(true);
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetTextureResource(L"T_Field");
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);

		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_CUBE");
		
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}

	void Ground::UpdateTexture()
	{
		SetVertices();
		auto ptrDraw = GetComponent<PNTStaticDraw>();
		ptrDraw->UpdateVertices(m_vertices);
	}

	void Ground::SetVertices()
	{
		for (int i = 0; i < m_vertices.size(); i += 4)
		{
			int s = i / 4;
			float xScale = (s >= 2 ? (s < 4 ? m_sPtrTrans->GetScale().z : m_sPtrTrans->GetScale().x) : m_sPtrTrans->GetScale().x) / 4;
			float yScale = (s >= 2 ? (s >= 4 ? m_sPtrTrans->GetScale().z : m_sPtrTrans->GetScale().y) : m_sPtrTrans->GetScale().y) / 4;
			m_vertices[i].textureCoordinate = Vec2(0, 0);
			m_vertices[i + 1].textureCoordinate = Vec2(0, yScale);
			m_vertices[i + 2].textureCoordinate = Vec2(xScale, yScale);
			m_vertices[i + 3].textureCoordinate = Vec2(xScale, 0);
		}
	}
}
