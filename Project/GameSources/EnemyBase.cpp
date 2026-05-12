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
        PointMove(GetThis<EnemyBase>(), 1.0f);
        DebugString();
    }

    // ガチ徘徊
    void EnemyBase::Move
    (
        const shared_ptr<GameObject>& gameObject,
        float speed
    )
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

    // 円状に動く
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

    // 4つのポイントを置いてランダムに動く
    void EnemyBase::PointMove
    (
        const shared_ptr<GameObject>& gameObject,
        float speed
    )
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

        // 移動距離
        float distance = 3.0f;

        // 経過時間を取得する
        auto deltaTime = App::GetApp()->GetElapsedTime();

        // 初期位置を取得したらそれ以降は絶対に位置を更新してはイケない
        if (m_isFirstTime == true)
        {
            m_InitialPosition = transPos;
            m_isFirstTime = false;
        }

        // 待機時間
        if (m_isStand == true)
        {
            //m_InitialStandTime = standingTime;
            if (m_InitialStandTime > ZERO_TIME)
            {
                m_InitialStandTime -= deltaTime;
            }
            else if (m_InitialStandTime <= ZERO_TIME)
            {
                m_isStand = false;
                m_isWandering = true;

                m_InitialWanderingTime = randTime;
                m_NumPoint = static_cast<Point>(rand() % 4);

                // 初期値を格納する
                m_TargetPosition = m_InitialPosition;

                switch (m_NumPoint)
                {
                case Point0: // 原点
                    m_TargetPosition = m_InitialPosition;
                    break;

                case Point1: // 原点から右
                    m_TargetPosition.x += distance;
                    break;

                case Point2: // 原点から下
                    m_TargetPosition.z -= distance;
                    break;

                case Point3: // 原点から斜め
                    m_TargetPosition.x += distance;
                    m_TargetPosition.z -= distance;
                    break;

                default:
                    break;
                }
            }
        }
        // 徘徊時間
        else if (m_isWandering == true)
        {
            // X軸の移動（正）
            if (transPos.x < m_TargetPosition.x) 
            {
                // 移動対象をX軸にプラスに移動させる
                transPos.x += speed * deltaTime;
                // 移動しているときにもし点を越してしまったら
                if (transPos.x > m_TargetPosition.x)
                {
                    transPos.x = m_TargetPosition.x;
                }
            }
            // X軸の移動（負）
            else if (transPos.x > m_TargetPosition.x) 
            {
                // 移動対象をX軸にマイナスに移動させる
                transPos.x -= speed * deltaTime;
                // 移動しているときにもし点を越してしまったら
                if (transPos.x < m_TargetPosition.x)
                {
                    transPos.x = m_TargetPosition.x;
                }
            }

            // Z軸の移動（正）
            if (transPos.z < m_TargetPosition.z) 
            {
                // 移動対象をZ軸にプラスに移動させる
                transPos.z += speed * deltaTime;
                // 移動しているときにもし点を越してしまったら
                if (transPos.z > m_TargetPosition.z)
                {
                    transPos.z = m_TargetPosition.z;
                }
            }
            // Z軸の移動（負）
            else if (transPos.z > m_TargetPosition.z) 
            {
                // 移動対象をZ軸にマイナスに移動させる
                transPos.z -= speed * deltaTime;
                // 移動しているときにもし点を越してしまったら
                if (transPos.z < m_TargetPosition.z)
                {
                    transPos.z = m_TargetPosition.z;
                }
            }

            // どれかの点に到達したとき
            if (transPos.x == m_TargetPosition.x &&
                transPos.z == m_TargetPosition.z)
            {
                // また待機時間に戻す
                m_isWandering = false;
                m_isStand = true;

                m_InitialStandTime = randTime;
            }


            // 敵の位置を最終的に更新する
            transComp->SetPosition(transPos);
        }
    }

    void EnemyBase::DebugString()
    {
        auto transComp = GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        GameManager::Instance().AddDebugStr(L"m_InitialStandTime", m_InitialStandTime);
        GameManager::Instance().AddDebugStr(L"m_InitialWanderingTime", m_InitialWanderingTime);
        GameManager::Instance().AddDebugStr(L"EnemyPositionX", transPos.x);
        GameManager::Instance().AddDebugStr(L"EnemyPositionY", transPos.y);
        GameManager::Instance().AddDebugStr(L"EnemyPositionZ", transPos.z);
        GameManager::Instance().AddDebugStr(L"EnemyInitialPositionX", m_InitialPosition.x);
        GameManager::Instance().AddDebugStr(L"EnemyInitialPositionY", m_InitialPosition.y);
        GameManager::Instance().AddDebugStr(L"EnemyInitialPositionZ", m_InitialPosition.z);
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