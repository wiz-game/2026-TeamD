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

		ptrDraw->SetMeshResource(L"M_Grand");
		ptrDraw->SetTextureResource(L"T_Grand");

		ptrDraw->SetOwnShadowActive(true);
		
		ptrShadow->SetMeshResource(L"M_Grand");

		m_sPtrTrans->SetScale(m_objectParam.GetScale());
		m_sPtrTrans->SetQuaternion(m_objectParam.GetQuaternion());
		m_sPtrTrans->SetPosition(m_objectParam.GetPosition());
		
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);

		// モデルとトランスフォーム間の差分行列
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.5f),
			Vec3(0.0f,  0.0f, 0.0f),
			Vec3(0.0f,  0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		ptrDraw->SetMeshToTransformMatrix(spanMat);

	}
}
