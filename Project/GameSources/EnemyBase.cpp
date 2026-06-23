#include "stdafx.h"
#include "Project.h"
#include "GameManager.h"

namespace basecross
{
    EnemyBase::EnemyBase(const shared_ptr<Stage>& stage, const STRUCT_ObjectParam& objectParam) :
        GameObject(stage, objectParam),
        m_InitialWanderingTime(0.0f),
        m_RandRotation(0.0f),
        m_WanderSpeed(1.0f),
        m_isWandering(false),
        m_InitialStandTime(0.0f),
        m_isStand(true),
        m_angle(0.0f),
        m_TargetPosition(),
        m_isFirstTime(true),
        m_Detection(true),
        m_LaunchofDirtCoolDown(0.0f),
        m_InitCoolDown(3.0f),
        m_isRotated(false),
        m_rotationSpeed(2.0f),
        m_rotY(0.0f),
        m_canGoLeft(true),
        m_canGoRight(true),
        m_canGoForward(true),
        m_targetRotY(0.0f),
        m_rayRange(1.6f),

        // 移動速度
        m_Speed(1.4f),
        m_rotToHeadLeap(0.1f)
    {}

    void EnemyBase::OnCreate()
    {

    }

    void EnemyBase::OnUpdate()
    {

    }

    void EnemyBase::DebugString()
    {
        auto transComp = GetComponent<Transform>();
        auto transPos = transComp->GetPosition();
        auto transRot = transComp->GetRotation();

        GameManager::Instance().AddDebugStr(L"Detection", m_Detection);
        GameManager::Instance().AddDebugStr(L"isRotated", m_isRotated);
        GameManager::Instance().AddDebugStr(L"EnemyRotY", transRot.y);
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
        // 現在のステージの情報を取得する
        auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
        if (stage == nullptr)
        {
            return;
        }

        // プレイヤーの情報を取得する
        auto player = stage->GetSharedGameObject<Player>(L"Player");
        if (player == nullptr)
        {
            return;
        }
        auto playerPos = player->GetComponent<Transform>()->GetPosition();

        // 引数の情報を取得する
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();
        auto transRot = transComp->GetRotation();

        // プレイヤーと自身の位置を計算する
        auto distancePos_X = playerPos.x - transPos.x;
        auto distancePos_Y = playerPos.y - transPos.y;
        auto distancePos_Z = playerPos.z - transPos.z;

        float distance = sqrt((distancePos_X * distancePos_X) + (distancePos_Y * distancePos_Y) + (distancePos_Z * distancePos_Z));

        float angleX = distancePos_X / distance;
        float angleY = distancePos_Y / distance;
        float angleZ = distancePos_Z / distance;

        auto forward = transComp->GetForward();
        forward.normalize();

        auto dot = (forward.x * angleX) + (forward.y * angleY) + (forward.z * angleZ);

        const float radius = 7.0f, rangeHeight = 0.3f;

        Vec3 startReyPos = Vec3(transPos.x, 0.3f, transPos.z);

        Vec3 endPSp(playerPos.x, transPos.y + rangeHeight, playerPos.z);

        float dotAngle = XMConvertToRadians(45.0f);

        if (distance < radius && dot >= cosf(dotAngle))
        {
            m_Detection = false;
            for (auto obj : stage->GetGameObjectVec())
            {
                if (obj == gameObject)
                {
                    continue;
                }

                auto drawComp = obj->GetComponent<PNTBoneModelDraw>(false);
                if (drawComp)
                {
                    Vec3 hitPoint;
                    TRIANGLE tri;
                    size_t index;
                    if (drawComp->HitTestStaticMeshSegmentTrianglesToAffine(startReyPos, endPSp, hitPoint, tri, index))
                    {
                        float disX = 0.0f, disZ = 0.0f, len = 0.0f;
                        disX = hitPoint.x - playerPos.x;
                        disZ = hitPoint.z - playerPos.z;
                        Vec3 dir(disX, 0.0f, disZ);
                        dir.length();
                        if (len <= m_rayRange)
                        {
                            m_Detection = true;
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            m_Detection = false;
        }
        // デバッグ文字
        GameManager::Instance().AddDebugStr(L"m_Detection", m_Detection);
        GameManager::Instance().AddDebugStr(L"dot", dot);
    }

    // 追跡AI
    void EnemyBase::Tracking(const shared_ptr<GameObject>& gameObject,float speed)
    {
        // 引数の情報を取得する
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();

        // ステージ情報を取得する
        auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
        if (stage == nullptr)
        {
            return;
        }

        // プレイヤーの情報を取得する
        auto player = stage->GetSharedGameObject<Player>(L"Player");
        if (player == nullptr)
        {
            return;
        }
        auto playerComp = player->GetComponent<Transform>();
        // プレイヤーの位置を取得する
        auto playerPos = playerComp->GetPosition();

        // 索敵しているとき
        if (m_Detection == true)
        {
            // プレイヤーと敵の位置を計算する
            float distanceX = playerPos.x - transPos.x;
            float distanceY = playerPos.y - transPos.y;
            float distanceZ = playerPos.z - transPos.z;
            float distance = sqrt((distanceX * distanceX) + (distanceY * distanceY) + (distanceZ * distanceZ));

            // 向き用
            float angleX = 0.0f, angleZ = 0.0f;
            float angleY = atan2f(distanceX, distanceZ);
            transComp->SetRotation(angleX, angleY, angleZ);

            // 向きを求める
            float forwardX = distanceX / distance;
            float forwardY = 0.0f;
            float forwardZ = distanceZ / distance;
               
            // 位置を更新する
            transPos.x += forwardX * App::GetApp()->GetElapsedTime() * speed;
            transPos.z += forwardZ * App::GetApp()->GetElapsedTime() * speed;
            // 位置を設定する
            transComp->SetPosition(transPos);
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

        Vec3 targetVec = playerPos - objPos;
        targetVec.y = 0.0f;
        targetVec.normalize();

        objPos.x += targetVec.x * stalkerSpeed * App::GetApp()->GetElapsedTime();
        objPos.z += targetVec.z * stalkerSpeed * App::GetApp()->GetElapsedTime();

        objComp->SetPosition(objPos);

        GetBehavior<UtilBehavior>()->RotToHead(targetVec, m_rotToHeadLeap);
    }

    void EnemyBase::MazeWandering(const shared_ptr<GameObject>& gameObject)
    {
        auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
        auto transComp = gameObject->GetComponent<Transform>();
        auto transPos = transComp->GetPosition();
        auto transRot = transComp->GetRotation();
        auto drawComp = gameObject->GetComponent<PNTBoneModelDraw>();

        float forward_X = 0.0f, forward_Z = 0.0f;
        // forward_Xとforward_Zを格納するための変数
        float rot_X = 0.0f, rot_Z = 0.0f;

        m_canGoLeft = true;
        m_canGoRight = true;
        m_canGoForward = true;

        // 右
        float forward_RX = transPos.x + ((sinf(m_rotY) + XMConvertToRadians(90.0f))) * m_rayRange;
        float forward_RZ = transPos.z + ((cosf(m_rotY) + XMConvertToRadians(90.0f))) * m_rayRange;

        // 左
        float forward_LX = transPos.x + ((sinf(m_rotY)-XMConvertToRadians(90.0f))) * m_rayRange;
        float forward_LZ = transPos.z + ((cosf(m_rotY)-XMConvertToRadians(90.0f))) * m_rayRange;

        Vec3 endRSp(forward_RX, transPos.y, forward_RZ);
        Vec3 endLSp(forward_LX, transPos.y, forward_LZ);

        // 正面
        float forward_FX = transPos.x + (sinf(m_rotY) * m_rayRange);
        float forward_FZ = transPos.z + (cosf(m_rotY) * m_rayRange);

        Vec3 startReyPos = Vec3(transPos.x, 0.3f, transPos.z);
        Vec3 endFSp(forward_FX, transPos.y, forward_FZ);

        float minT = 1.0f;
        // 全てのゲームオブジェクトを探す
        for (auto& obj : stage->GetGameObjectVec())
        {
            // 自身が引数のgameObjectと等しければ次にいく
            if (obj == gameObject)
            {
                continue;
            }

            auto objDrawComp = obj->GetComponent<PNTStaticDraw>(false);
            if (objDrawComp)
            {
                Vec3 hitPoint;
                TRIANGLE tri;
                size_t index;
                if (obj->FindTag(L"Wall") && m_isRotated == false)
                {
                    float dirX = 0.0f, dirZ = 0.0f, len = 0.0f;

                    if (objDrawComp->HitTestStaticMeshSegmentTrianglesToAffine(startReyPos, endLSp, hitPoint, tri, index))
                    {
                        // 左
                        dirX = hitPoint.x - transPos.x;
                        dirZ = hitPoint.z - transPos.z;
                        Vec3 dir(dirX, 0.0f, dirZ);
                        len = dir.length();

                        if (len <= m_rayRange)
                        {
                            // 壁に触れた
                            m_canGoLeft = false;
                        }
                    }

                    if (objDrawComp->HitTestStaticMeshSegmentTrianglesToAffine(startReyPos, endRSp, hitPoint, tri, index))
                    {
                        // 右
                        dirX = hitPoint.x - transPos.x;
                        dirZ = hitPoint.z - transPos.z;
                        Vec3 dir(dirX, 0.0f, dirZ);
                        len = dir.length();

                        if (len <= m_rayRange)
                        {
                            // 壁に触れた
                            m_canGoRight = false;
                        }
                    }

                    if (objDrawComp->HitTestStaticMeshSegmentTrianglesToAffine(startReyPos, endFSp, hitPoint, tri, index))
                    {
                        // 正面
                        dirX = hitPoint.x - transPos.x;
                        dirZ = hitPoint.z - transPos.z;
                        Vec3 dir(dirX, 0.0f, dirZ);
                        len = dir.length();

                        if (len <= m_rayRange)
                        {
                            // 壁に触れた
                            m_canGoForward = false;
                        }
                    }
                }
            }
        }

        if (m_isRotated == false && m_canGoForward == false)
        {
            // 左右どちらも行けるならどちらかをランダムに決定して移動
            if ((m_canGoLeft == true && m_canGoRight == true) && m_isRotated == false)
            {
                m_NumRandRot = static_cast<ENUM_RANDOMROT>(rand() % RandomRotNum);
                switch (m_NumRandRot)
                {
                case RandomRotL:
                    //m_rotY -= XMConvertToRadians(90.0f);
                    m_targetRotY = m_rotY - XMConvertToRadians(90.0f);
                    break;

                case RandomRotR:
                    //m_rotY += XMConvertToRadians(90.0f);
                    m_targetRotY = m_rotY + XMConvertToRadians(90.0f);
                    break;

                default:
                    break;
                }
                m_isRotated = true;
            }
            // 片方のみならその方向に決定して移動
            // 左回転
            else if (m_canGoLeft == true)
            {
                //m_rotY -= XMConvertToRadians(90.0f);
                m_targetRotY = m_rotY - XMConvertToRadians(90.0f);
                m_isRotated = true;
            }
            // 右回転
            else if (m_canGoRight == true)
            {
                //m_rotY += XMConvertToRadians(90.0f);
                m_targetRotY = m_rotY + XMConvertToRadians(90.0f);
                m_isRotated = true;
            }
            // どちらもダメなら自分の真後ろに決定して移動
            else
            {
                //m_rotY += XMConvertToRadians(180.0f);
                m_targetRotY = m_rotY + XMConvertToRadians(180.0f);
                m_isRotated = true;
            }

        }

        const float LIMIT_ANGLE = 360.0f,ZERO_ANGLE = 0.0f;
        if (m_isRotated)
        {
            // 現在の角度より目標の角度が大きい場合
            if(m_rotY < m_targetRotY)
            {
                m_rotY += App::GetApp()->GetElapsedTime() * m_rotationSpeed;

                // 目標の角度を超過してしまった場合
                if (m_rotY >= m_targetRotY)
                {
                    m_rotY = m_targetRotY;
                    m_isRotated = false;
                }
            }

            // 目標の角度より現在の角度が大きい場合
            if (m_rotY > m_targetRotY)
            {
                m_rotY -= App::GetApp()->GetElapsedTime() * m_rotationSpeed;

                // 目標の角度より小さくなってしまった場合
                if (m_rotY <= m_targetRotY)
                {
                    m_rotY = m_targetRotY;
                    m_isRotated = false;
                }
            }

            // 360°以上になったら0°に戻す
            if (m_rotY >= XMConvertToRadians(LIMIT_ANGLE))
            {
                m_rotY = XMConvertToRadians(ZERO_ANGLE);
            }

            if (m_rotY <= XMConvertToRadians(-LIMIT_ANGLE))
            {
                m_rotY = XMConvertToRadians(ZERO_ANGLE);
            }

            transComp->SetRotation(0.0f, m_rotY, 0.0f);
        }
        else
        {
            transPos.x = transPos.x + (sinf(m_rotY) * m_Speed * App::GetApp()->GetElapsedTime());
            transPos.z = transPos.z + (cosf(m_rotY) * m_Speed * App::GetApp()->GetElapsedTime());
            transComp->SetPosition(transPos);
        }

        transComp->SetRotation(0.0f, m_rotY, 0.0f);
    }

    void EnemyBase::OnCollisionEnter(shared_ptr<GameObject>& Other)
    {
        auto bubble = dynamic_pointer_cast<Bubble>(Other);
        if (bubble == nullptr)
        {
            return;
        }

        auto force = bubble->GetBubbleHP();
        bool soup = bubble->GetBubblePowerUp();
        if (Other->FindTag(L"Bubble") && soup == true)
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