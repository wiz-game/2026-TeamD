#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Mushroom::Mushroom
	(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam
	) :
		GameObject(StagePtr, objectParam)
	{};

	Mushroom::~Mushroom()
	{
	}

	void Mushroom::OnCreate()
	{
		AddTag(L"Mushroom");
		SetIsEditorSave(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"M_Mushroom");
		ptrDraw->SetTextureResource(L"T_Mushroom");

		auto ptrShadow = AddComponent<Shadowmap>();
		ptrDraw->SetOwnShadowActive(true);
		ptrShadow->SetMeshResource(L"M_Mushroom");

		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());

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

		auto collision = AddComponent<CollisionCapsule>();
		collision->SetFixed(true);

	}
}