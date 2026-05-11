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
		const int& Id,
		const Vec3& Scale,
		const Quat& Quaternion,
		const Vec3& Position
	) :
		GameObject(StagePtr, Id)
	{
		m_scale = Scale;
		m_quaternion = Quaternion;
		m_position = Position;
	}

	Ground::~Ground()
	{
	}

	void Ground::OnCreate()
	{
		AddTag(L"PlayerUnderRay");
		AddTag(L"Ground");
		AddTag(L"BossLastRandTarget");
		SetAlphaActive(true);
		
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		auto ptrShadow = AddComponent<Shadowmap>();
		m_sPtrTrans = AddComponent<Transform>();

		Vec3 scale;
		Mat4x4 spanMat;

		ptrDraw->SetMeshResource(L"Stage_Floor");
		ptrDraw->SetTextureResource(L"T_Stage_Floor");

		ptrDraw->SetOwnShadowActive(true);

		scale = Vec3(13.0f * m_scale.x, 21.0f * m_scale.y, 13.0f * m_scale.z);
		spanMat.affineTransformation
		(
			Vec3(0.43f, 0.267f, 0.43f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		ptrShadow->SetMeshResource(L"Stage_Floor");

		m_sPtrTrans->SetScale(scale);
		m_sPtrTrans->SetQuaternion(m_quaternion);
		m_sPtrTrans->SetPosition(m_position);

		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}
}
