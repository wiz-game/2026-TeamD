#include "stdafx.h"
#include "Project.h"
#include "GameManager.h"

namespace basecross
{
    void EnemyBase::OnCreate()
    {
    }

	void EnemyBase::OnUpdate()
	{
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
                m_wandering = static_cast<Wandering>(rand() % Number);
            }
        }
        // 徘徊時間
        else if (m_isWandering == true)
        {
            //m_InitialWanderingTime = wanderingTime;
            if (m_InitialWanderingTime > ZERO_TIME)
            {
                m_InitialWanderingTime -= App::GetApp()->GetElapsedTime();

                // ここに徘徊処理を書く
                switch (m_wandering)
                {
                case move_x_puls:
                    transPos.x += deltaTime * speed;
                    break;

                case move_x_minus:
                    transPos.x -= deltaTime * speed;
                    break;
                    
                case move_z_puls:
                    transPos.z += deltaTime * speed;
                    break;

                case move_z_minus:
                    transPos.z -= deltaTime * speed;
                    break;

                default:
                    break;
                }
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

    void EnemyBase::DebugString()
    {
        GameManager::Instance().AddDebugStr(L"m_InitialStandTime", m_InitialStandTime);
        GameManager::Instance().AddDebugStr(L"m_InitialWanderingTime", m_InitialWanderingTime);
    }
}