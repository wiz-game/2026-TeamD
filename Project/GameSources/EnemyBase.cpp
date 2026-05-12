#include "stdafx.h"
#include "Project.h"
#include "GameManager.h"

namespace basecross
{
    void EnemyBase::OnCreate()
    {
        DebugDraw();
    }

	void EnemyBase::OnUpdate()
	{
        // Move(GetThis<EnemyBase>(), 2.0f);
        CircleMove(GetThis<EnemyBase>(), 1.0f,1.0f);
        DebugString();
	}

	// 敵共通の移動の動きを書く
	void EnemyBase::Move(const shared_ptr<GameObject>& gameObject,float speed)
	{
        // そのゲームオブジェクトの情報を取得する
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        // 時間（乱数）
        float randTime = rand() % 3 + 1;
        float randRotation = rand() % 361;

        // ゼロ
        const float ZERO_TIME = 0.0f;

        // タイマースピード
        float timerSpeed = 1.0f;

        // 経過時間を取得する
        auto deltaTime = App::GetApp()->GetElapsedTime();

        // 待機時間
        if (m_isStand == true)
        {
            //m_InitialStandTime = standingTime;
            if (m_InitialStandTime > ZERO_TIME)
            {
                m_InitialStandTime -= App::GetApp()->GetElapsedTime();
            }
            else if (m_InitialStandTime <= ZERO_TIME)
            {
                m_isStand = false;
                m_isWandering = true;

                m_InitialWanderingTime = randTime;
                m_RandRotation = randRotation;
            }
        }
        // 徘徊時間
        else if (m_isWandering == true)
        {
            if (m_InitialWanderingTime > ZERO_TIME)
            {
                m_InitialWanderingTime -= App::GetApp()->GetElapsedTime();
                float rad = XMConvertToRadians(m_RandRotation);
                // 徘徊をランダムにする
                transPos.x += cosf(rad) * deltaTime * speed;
                transPos.z += sinf(rad) * deltaTime * speed;
            }
            else if (m_InitialWanderingTime <= ZERO_TIME)
            {
                m_isWandering = false;
                m_isStand = true;

                m_InitialStandTime = randTime;
            }
        }

        // 敵の位置を最終的に更新する
        transComp->SetPosition(transPos);
    }

    void EnemyBase::CircleMove
    (
        const shared_ptr<GameObject>& gameObject,
        float radius,
        float moveSpeed
    )
    {
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        // 角度の値を増やす
        m_angle += moveSpeed * App::GetApp()->GetElapsedTime();

        // 現在の位置を更新する（カメラの回転を参考にしました）
        // radius：回転する半径を決めます
        transPos.x = cosf(m_angle) * radius;
        transPos.z = sinf(m_angle) * radius;

        transComp->SetPosition(transPos.x, transPos.y, transPos.z);

        float angle = atan2f(transPos.x, transPos.z);
        transComp->SetRotation(0.0f, angle, 0.0f);
    }

    void EnemyBase::DebugString()
    {
        GameManager::Instance().AddDebugStr(L"m_InitialStandTime", m_InitialStandTime);
        GameManager::Instance().AddDebugStr(L"m_InitialWanderingTime", m_InitialWanderingTime);
    }

    void EnemyBase::DebugDraw()
    {
        auto transComp = AddComponent<Transform>();
        transComp->SetPosition(0.0f, 61.0f, 0.0f);

        auto drawComp = AddComponent<PNTStaticDraw>();
        drawComp->SetMeshResource(L"DEFAULT_CUBE");
        drawComp->SetDrawActive(true);
    }
}