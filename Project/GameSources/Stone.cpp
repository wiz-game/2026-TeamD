#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Stone::Stone(
		const shared_ptr<Stage>& stage,
		const STRUCT_ObjectParam& objectParam
	) :
		GameObject(stage, objectParam)
	{
	}

	Stone::~Stone()
	{
	}

	void Stone::OnCreate()
	{
		AddTag(L"Stone");
		SetIsEditorSave(true);

		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"M_Stone");
		ptrDraw->SetTextureResource(L"T_Stone");
		
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"M_Stone");
		ptrShadow->SetDrawActive(true);

		Mat4x4 spanMat;
		spanMat.affineTransformation
		(
			Vec3(0.75f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrShadow->SetMeshToTransformMatrix(spanMat);
	}
}