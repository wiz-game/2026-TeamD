#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	Bubble::Bubble(const shared_ptr<Stage>& stage, const shared_ptr<GameObject>& parent) :
		GameObject(stage),
		m_parent(parent),
		m_speed(1.5f),
		m_speedRatio(0.0f),
		m_initialVelocity(2.5f),
		m_currentVelocity(m_initialVelocity),
		m_upwardVelocity(0.0f),
		m_isTimeStart(false),
		m_limitTime(0.75f),
		m_isSpawnedTrampoline(false),
		m_isHit(false)
	{

	}

	Bubble::~Bubble()
	{

	}

	void Bubble::OnCreate()
	{		
		AddTag(L"Bubble");

		// 親の情報の取得
		auto parentLock = m_parent.lock();
		m_parentForward = parentLock->GetComponent<Transform>()->GetForward();
		m_parentForward.normalize();
		auto parentPos = parentLock->GetComponent<Transform>()->GetPosition();
		
		m_trans = GetComponent<Transform>();
		Vec3 spawnPos = Vec3(parentPos.x, parentPos.y + 1.0f, parentPos.z) + m_parentForward * 1.25f;
		m_dir = GetCameraForward();

		m_trans->SetPosition(spawnPos);
		m_trans->SetScale(Vec3(1.25f));
		// m_trans->SetQuaternion()

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(false);

		// 透明化処理
		SetAlphaActive(true);

		m_draw = AddComponent<PNTStaticDraw>();
		m_draw->SetMeshResource(L"M_Bubble");
		m_draw->SetTextureResource(L"T_Bubble");
		m_draw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.3f));
		// m_draw->SetSpecular(Col4(1.0f));

		// モデルとトランスフォーム間の差分行列
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.45f,  0.45f,   0.45f),
			Vec3(0.0f,  0.0f,   0.0f),
			Vec3(0.0f, XM_PI,   0.0f),
			Vec3(0.0f, -0.4f,   0.0f)
		);

		m_draw->SetMeshToTransformMatrix(spanMat);
	}

	void Bubble::OnUpdate()
	{
		BubbleMove();
	}

	void Bubble::BubbleMove()
	{
		auto& app = App::GetApp();
		auto elapsed = app->GetElapsedTime();
		auto pos = m_trans->GetPosition();
		auto parent = m_parent.lock();
		auto player = dynamic_pointer_cast<Player>(parent);

		float velocityZero = 0.0f;
		float decelerationStartRatio = 0.2f;

		// 現在速度が0になるまで
		if (m_currentVelocity > velocityZero)
		{
			// 減少速度
			m_currentVelocity -= m_speed * elapsed;
			m_currentVelocity = max(m_currentVelocity, velocityZero);
			// 速度の割合
			m_speedRatio = m_currentVelocity / m_initialVelocity;

			if (player->GetTargetMode())
			{
				pos += m_dir * (m_currentVelocity * elapsed);
			}
			else
			{
				pos += m_parentForward * (m_currentVelocity * elapsed);
			}
		}

		// 現在の速度の割合が始めるぐらいの割合になったら上昇を始める
		if (m_speedRatio < decelerationStartRatio)
		{
			m_isTimeStart = true;

			// 0.5 -> 0.0fに向かうほど t: 0 -> 1
			auto t = (decelerationStartRatio - m_speedRatio) / decelerationStartRatio;
			t = clamp(t,0.0f, 1.0f);

			// 1から2までの補間係数
			m_upwardVelocity = 1.0f + (1.25f - 1.0f) * t;

			pos.y += m_upwardVelocity * elapsed;
		}

		if (m_isTimeStart)
		{
			m_limitTime -= 1.0f * elapsed;
		}

		if (m_limitTime < 0.0f)
		{
			GetStage()->RemoveGameObject<Bubble>(GetThis<Bubble>());
			return;
		}

		m_trans->SetPosition(pos);
	}

	Vec3 Bubble::GetCameraForward()
	{
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto PtrCamera = stage->GetView()->GetTargetCamera();
		Vec3 at = PtrCamera->GetAt();
		Vec3 eye = PtrCamera->GetEye();
		Vec3 forward = at - eye;
		forward.normalize();
		
		return forward;
	}

	void Bubble::OnCollisionEnter(shared_ptr<GameObject>& Other)
	{
		if (!Other->FindTag(L"Ground")) return;

		if (m_isSpawnedTrampoline) return;
		if (m_isHit) return;
		m_isHit = true;

		auto range = 1.5f;
		auto stage = GetStage();
		auto ground = dynamic_pointer_cast<Ground>(Other);
		auto myPos = m_trans->GetPosition();
		auto objvec = stage->GetGameObjectVec();

		bool found = false;

		// 地面に当たった瞬間近くのトランポリン泡を探す
		for (auto& obj : objvec)
		{
			if (obj->FindTag(L"TranmpolineBase"))
			{
				auto tramp = dynamic_pointer_cast<TrampolineBubbles>(obj);
				auto trampTrans = tramp->GetComponent<Transform>();
				if (trampTrans)
				{
					float dist = (myPos - trampTrans->GetPosition()).length();
					
					if (dist < range)
					{
						found = true;
						tramp->AddbubbleCount();
						break;
					}
				}
			}
		}
		if (!found && ground)
		{
			m_isSpawnedTrampoline = true;
			auto bubblePos = m_trans->GetPosition();
			GetStage()->AddGameObject<TrampolineBubbles>(Vec3(bubblePos));
		}

		GetStage()->RemoveGameObject<Bubble>(GetThis<Bubble>());
	}

	ViewBubble::ViewBubble(const shared_ptr<Stage>& stage, const vector<Vec3> *vertices) :
		GameObject(stage),
		m_vertices(vertices)
	{

	}

	ViewBubble::~ViewBubble()
	{
	}

	void ViewBubble::OnCreate()
	{
		m_trans = GetComponent<Transform>();
		m_trans->SetScale(Vec3(0.1f));

		m_draw = AddComponent<PNTStaticInstanceDraw>();
		m_draw->SetMeshResource(L"M_Bubble");
		m_draw->SetTextureResource(L"T_Bubble");
	}

	void ViewBubble::OnUpdate()
	{		
		CreateInstance();
	}

	void ViewBubble::CreateInstance()
	{		
		m_draw->ClearMatrixVec();

		for (const auto& pos : *m_vertices)
		{
			Mat4x4 mat;
			mat.affineTransformation(
				Vec3(0.1f),
				Vec3(0.0f),
				Quat(0.0f),
				pos
			);
			m_draw->AddMatrix(mat);
		}
	}
}
