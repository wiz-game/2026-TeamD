#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Mushroom::Mushroom(
		const shared_ptr<Stage>& StagePtr,
		const int& Id,
		const Vec3& Scale,
		const Quat& Quatition,
		const Vec3& Position
	) :
		GameObject(StagePtr, Id)
	{
		m_scale = Scale;
		m_quaternion = Quatition;
		m_position = Position;
	};

	Mushroom::~Mushroom()
	{

	}

	void Mushroom::OnCreate()
	{
		AddTag(L"Mushroom");

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"M_Mushroom");
		ptrDraw->SetTextureResource(L"T_Mushroom");

		auto ptrShadow = AddComponent<Shadowmap>();
		ptrDraw->SetOwnShadowActive(true);

		m_sPtrTrans = AddComponent<Transform>();
		m_sPtrTrans->SetPosition(m_position);
		m_sPtrTrans->SetQuaternion(m_quaternion);
		m_sPtrTrans->SetScale(m_scale);

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