#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Dirt::Dirt(
		const shared_ptr<Stage>& StagePtr,
		const STRUCT_ObjectParam& objectParam
	):
		GameObject(StagePtr, objectParam),
		m_nowDeleteCount(0),
		m_alphaColor(1.0f),
		m_HP(10.0f)
	{
	}

	Dirt::~Dirt()
	{
	}

	void Dirt::OnCreate()
	{
		AddTag(L"Dirt");
		SetIsEditorSave(true);

		m_trans = AddComponent<Transform>();
		m_trans->SetScale(m_objectParam.GetScale());
		m_trans->SetQuaternion(m_objectParam.GetQuaternion());
		m_trans->SetPosition(m_objectParam.GetPosition());

		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"M_Sludge");
		m_draw->SetTextureResource(L"T_Sludge");

		//auto ptrShadow = AddComponent<Shadowmap>();
		//m_draw->SetOwnShadowActive(true);
		//ptrShadow->SetMeshResource(L"M_Sludge");

		auto col = AddComponent<CollisionObb>();
		// col->SetDrawActive(true);
		col->SetAfterCollision(AfterCollision::None);

		// モデルとトランスフォーム間の差分行列
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.45f, 6.7f, 0.45f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.05f,-0.5f, 0.0f)
		);

		m_draw->SetMeshToTransformMatrix(spanMat);
		//ptrShadow->SetMeshToTransformMatrix(spanMat);
	}

	void Dirt::OnUpdate()
	{
		if(m_HP <= 0.0f)
		{
			GetStage()->RemoveGameObject<Dirt>(GetThis<Dirt>());
		}
	}

	void Dirt::OnUpdate2()
	{

	}

	void Dirt::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
	}
}