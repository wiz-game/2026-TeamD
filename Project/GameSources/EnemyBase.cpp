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
                // 一度だけ乱数を格納する
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
            // ターゲット（点）までのベクトルを計算
            float diffX = m_TargetPosition.x - transPos.x;
            float diffZ = m_TargetPosition.z - transPos.z;

            // ターゲットまでの距離を計算
            float distance = sqrtf(diffX * diffX + diffZ * diffZ);

            if (distance > 0.05f)
            {
                float forward = atan2f(diffX, diffZ);
                transComp->SetRotation(0.0f, forward, 0.0f);

                transPos.x += (diffX / distance) * speed * deltaTime;
                transPos.z += (diffZ / distance) * speed * deltaTime;
            }
            // どれかの点に到達したとき
            else
            {
                // 強制的に点に重ねる
                transPos.x = m_TargetPosition.x;
                transPos.z = m_TargetPosition.z;

                // また待機時間に戻す
                m_isWandering = false;
                m_isStand = true;

                m_InitialStandTime = randTime;
            }

            // 敵の位置を最終的に更新する
            transComp->SetPosition(transPos);
        }
    }

    void EnemyBase::FunctionGravity(const shared_ptr<GameObject>& gameObject)
    {
        auto gravity = gameObject->GetComponent<Gravity>();
        if (gravity == nullptr)
        {
            return;
        }
    }

    void EnemyBase::DebugString()
    {
        auto transComp = GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        GameManager::Instance().AddDebugStr(L"Detection", m_Detection);
    }

    void EnemyBase::Died(const shared_ptr<GameObject>& gameObject)
    {
        // 自身の位置を取得する
        auto objComp = gameObject->GetComponent<Transform>();
        auto objPos = objComp->GetPosition();

        const float DIED_HP = 0.0f;
        if (m_EnemyHP <= DIED_HP)
        {
            auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
            stage->RemoveGameObject<GameObject>(GetThis<GameObject>());

            // 石鹸を出す
            auto soap = stage->AddGameObject<PowerUpSoap>();
            auto soapComp = soap->GetComponent<Transform>();
            soap->SetVecPosition(Vec3(objPos.x, objPos.y + 1, objPos.z));
            //soapComp->SetPosition(objPos.x, objPos.y + 1, objPos.z);
        }
    }

    // 索敵範囲
    void EnemyBase::DetectionRange(const shared_ptr<GameObject>& gameObject)
    {
        auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
        if (stage == nullptr)
        {
            return;
        }

        auto player = stage->GetSharedGameObject<Player>(L"Player");
        if (player == nullptr)
        {
            return;
        }
        auto playerPos = player->GetComponent<Transform>()->GetPosition();

        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        auto distancePos_X = playerPos.x - transPos.x;
        auto distancePos_Y = playerPos.y - transPos.y;
        auto distancePos_Z = playerPos.z - transPos.z;

        float distance = sqrt((distancePos_X * distancePos_X) + (distancePos_Y * distancePos_Y) + (distancePos_Z * distancePos_Z));

        const float radius = 10.0f;

        if (distance < radius)
        {
            m_Detection = true;
            float forward = atan2f(distancePos_X, distancePos_Z);
            transComp->SetRotation(0.0f, forward, 0.0f);
        }
        else
        {
            m_Detection = false;
        }
    }

    // ストーカー
    void EnemyBase::Stalker(const shared_ptr<GameObject>& gameObject, float stalkerSpeed)
    {
        auto player = App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetSharedGameObject<Player>(L"Player");
        if (player == nullptr)
        {
            return;
        }

        auto playerComp = player->GetComponent<Transform>();
        auto playerPos = playerComp->GetPosition();

        auto objComp = gameObject->GetComponent<Transform>();
        auto objPos = objComp->GetPosition();

        float stalkerX = objPos.x + (playerPos.x - objPos.x) * stalkerSpeed * App::GetApp()->GetElapsedTime();
        //float stalkerY = objPos.y + (playerPos.y - objPos.y) * stalkerSpeed * App::GetApp()->GetElapsedTime();
        float stalkerZ = objPos.z + (playerPos.z - objPos.z) * stalkerSpeed * App::GetApp()->GetElapsedTime();

        objComp->SetPosition(stalkerX, 0.0f, stalkerZ);
    }

    // 泡を吐く
    void EnemyBase::DropDirt(const shared_ptr<GameObject>& gameObject)
    {
        auto objTrans = gameObject->GetComponent<Transform>();
        auto objPos = objTrans->GetPosition();
        STRUCT_ObjectParam objParm = STRUCT_ObjectParam(ENUM_ObjectID::Dirt,Vec3(1.0f),Quat(0.0f),Vec3(objPos));
        auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
        m_LaunchofDirtCoolDown -= App::GetApp()->GetElapsedTime();
        if (m_LaunchofDirtCoolDown <= 0.0f)
        {
            auto dirt = stage->AddGameObject<Dirt>(objParm);
            m_LaunchofDirtCoolDown = m_InitCoolDown;
        }
    }

    void EnemyBase::MazeWandering(const shared_ptr<GameObject>& gameObject,float moveSpeed)
    {
        auto objTrans = gameObject->GetComponent<Transform>();
        auto objPos = objTrans->GetPosition();
        auto objRot = objTrans->GetRotation();

        float rotDistance = 90.0f;
        const float ZERO_ROTATION = 0.0f,MAX_ROTATION = 360.0f;

        // 向いている方向に壁がある場合は90°回転する
        if (gameObject->FindTag(L"Ground"))
        {
            objRot.y += rotDistance * App::GetApp()->GetElapsedTime();

            if (objRot.y >= MAX_ROTATION)
            {
                objRot.y -= ZERO_ROTATION;
            }

            objTrans->SetRotation(objRot);
        }
    }

    void EnemyBase::OnCollisionEnter(shared_ptr<GameObject>& Other)
    {
        auto bubble = dynamic_pointer_cast<Bubble>(Other);
        if (bubble == nullptr)
        {
            return;
        }

        auto force = bubble->GetBubbleHP();
        if (Other->FindTag(L"Bubble"))
        {
            m_EnemyHP -= force;
        }
    }

    void EnemyBase::OnCollisionExecute(shared_ptr<GameObject>& Other)
    {

    }

    void EnemyBase::OnCollisionExit(shared_ptr<GameObject>& Other)
    {

    }
}