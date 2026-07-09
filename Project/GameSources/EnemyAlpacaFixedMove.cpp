/*!
@file EnemyAlpacaFixedMove.cpp
@brief 徘徊する敵キャラクター
*/

#include "stdafx.h"
#include "Project.h"
#include <cmath>

namespace basecross
{
	// 初期化
	void EnemyAlpacaFixedMove::OnCreate()
	{
		AddTag(L"Enemy");
		SetIsEditorSave(true);

		m_transform = AddComponent<Transform>();
		m_transform->SetPosition(m_objectParam.GetPosition());
		m_transform->SetScale(m_objectParam.GetScale());
		m_transform->SetQuaternion(m_objectParam.GetQuaternion());
		m_transform->SetRotation(0.0f, 0.0f, 0.0f);

		m_draw = AddComponent<PNTBoneModelDraw>();
		m_draw->SetMeshResource(L"DoroPaka");
		m_draw->SetTextureResource(L"T_DoroPaka_Body");
		m_draw->SetDrawActive(true);

		auto obb = AddComponent<CollisionObb>();
		obb->AddExcludeCollisionTag(L"Enemy");

		auto shadowComp = AddComponent<Shadowmap>();
		shadowComp->SetMeshResource(L"DoroPaka");
		shadowComp->SetDrawActive(true);

		Mat4x4 spanMat;
		spanMat.affineTransformation
		(
			Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		m_draw->SetMeshToTransformMatrix(spanMat);
		shadowComp->SetMeshToTransformMatrix(spanMat);

		auto loopFlag = true;
		m_draw->AddAnimation(L"Idle", 0, 65, loopFlag, 60.0f);
		m_draw->AddAnimation(L"Walk", 70, 80, loopFlag, 60.0f);

		m_draw->ChangeCurrentAnimation(L"Walk");
	}

	// 更新
	void EnemyAlpacaFixedMove::OnUpdate()
	{
		m_draw->UpdateAnimation(App::GetApp()->GetElapsedTime());
		PointMove(GetThis<EnemyAlpacaFixedMove>(), m_Speed);
	}
}
//end basecross
