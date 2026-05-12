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

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		auto ptrShadow = AddComponent<Shadowmap>();
		m_sPtrTrans = AddComponent<Transform>();

		ptrDraw->SetMeshResource(L"Stage_Floor");
		ptrDraw->SetTextureResource(L"Ground_Sozai");

		ptrDraw->SetOwnShadowActive(true);

		Mat4x4 spanMat;
		spanMat.affineTransformation
		(
			Vec3(0.43f, 0.267f, 0.43f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		ptrShadow->SetMeshResource(L"Stage_Floor");

		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());

		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}
}
