/*!
@file SoilWall.cpp
@brief 土の壁
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	SoilWall::SoilWall
	(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam
	) :
		GameObject(StagePtr, objectParam)
	{
	}

	SoilWall::~SoilWall()
	{
	}

	void SoilWall::OnCreate()
	{
		AddTag(L"SoilWall");
		AddTag(L"Fade");
		AddTag(L"Wall");
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
		//ptrDraw->CreateOriginalMesh(m_vertices, indices);
		//ptrDraw->SetOriginalMeshUse(true);
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetMeshResource(L"M_SoilWall");
		ptrDraw->SetTextureResource(L"T_SoilWall");
		ptrDraw->SetSamplerState(SamplerState::LinearWrap);

		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"M_SoilWall");

		// モデルとトランスフォーム間の差分行列
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.47f, 0.47f, 0.47f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		ptrDraw->SetMeshToTransformMatrix(spanMat);


		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}

	void SoilWall::OnUpdate()
	{
		m_stageObjectFade.UpdateFade(GetComponent<PNTStaticDraw>(), App::GetApp()->GetElapsedTime());
	}

	void SoilWall::UpdateTexture()
	{
		//SetVertices();
		//auto ptrDraw = GetComponent<PNTStaticDraw>();
		//ptrDraw->UpdateVertices(m_vertices);
	}

	void SoilWall::SetVertices()
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
