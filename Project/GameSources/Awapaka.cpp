#include "stdafx.h"
#include "Project.h"
#include "Awapaka.h"

namespace basecross
{
	void Awapaka::OnCreate()
	{
		m_transform = AddComponent<Transform>();
		m_transform->SetPosition(m_position);
		m_transform->SetRotation(m_rotation);
		m_transform->SetScale(Vec3(1.0f));

		m_pntDraw = AddComponent<PNTBoneModelDraw>();
		m_pntDraw->SetMeshResource(L"AwaPaka");
		m_pntDraw->SetTextureResource(L"T_AwaPaka_Body");
		m_pntDraw->SetDrawActive(true);
		m_pntDraw->SetOwnShadowActive(true);

		auto shadowComp = AddComponent<Shadowmap>();
		shadowComp->SetMeshResource(L"AwaPaka");
		shadowComp->SetDrawActive(true);

		auto loopFlag = true;
		m_pntDraw->AddAnimation(L"Idle", 0, 65, loopFlag, 60.0f);
		m_pntDraw->AddAnimation(L"Walk", 70, 80, loopFlag, 60.0f);
		m_pntDraw->AddAnimation(L"Bubble", 155, 30, loopFlag, 60.0f);
		m_pntDraw->AddAnimation(L"GameOver", 190, 60, !loopFlag, 60.0f);
		m_pntDraw->AddAnimation(L"GameClear", 255, 45, loopFlag, 60.0f);
		m_pntDraw->AddAnimation(L"Eat", 305, 50, !loopFlag, 30.0f);
		m_pntDraw->AddAnimation(L"ALL", 0, 355, loopFlag, 60.0f);
		m_pntDraw->ChangeCurrentAnimation(m_animetion);
	}

	void Awapaka::OnUpdate()
	{
		m_pntDraw->UpdateAnimation(App::GetApp()->GetElapsedTime());
	}
}
