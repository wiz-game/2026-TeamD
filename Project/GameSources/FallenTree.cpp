#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	FallenTree::FallenTree(
		const shared_ptr<Stage>& stage,
		const STRUCT_ObjectParam& objectParam

	) :
		GameObject(stage, objectParam)
	{
	}

	FallenTree::~FallenTree()
	{
	}

	void FallenTree::OnCreate()
	{
		AddTag(L"FallenTree");
		AddTag(L"Fade");
		SetIsEditorSave(true);
		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"M_FallenTree");
		ptrDraw->SetTextureResource(L"T_FallenTree");

		auto ptrShadow = AddComponent<Shadowmap>();
		ptrDraw->SetOwnShadowActive(true);
		ptrShadow->SetMeshResource(L"M_FallenTree");

		Mat4x4 spanMat;
		spanMat.affineTransformation
		(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		SetAlphaActive(true);
	}

	void FallenTree::OnUpdate()
	{
		m_stageObjectFade.UpdateFade(GetComponent<PNTStaticDraw>(), App::GetApp()->GetElapsedTime());
	}
}